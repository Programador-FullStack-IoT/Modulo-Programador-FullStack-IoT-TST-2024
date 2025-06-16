#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <RadioLib.h>  // Usamos RadioLib en lugar de LoRa.h
#include <Wire.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

// --- Configuración WiFi/MQTT ---
const char* WIFI_SSID      = "Fibertel-WiFi032";
const char* WIFI_PASS      = "vuelalto@67";
const char* MQTT_SERVER    = "telecomunicaciones.ddns.net";
const uint16_t MQTT_PORT   = 2480;
const char* TOPIC_COMANDOS = "omnisens/comandos/AQC_001";
const char* TOPIC_DATOS    = "omnisens/datos/AQC_001";

// --- Configuración Hardware LoRa ---
#define LORA_SS    5
#define LORA_RST   14
#define LORA_DIO0  26
#define LORA_DIO1  34  // Opcional para interrupciones

// Instancias globales
WiFiClient netClient;
PubSubClient mqtt(netClient);
SX1278 radio = new Module(LORA_SS, LORA_DIO0, LORA_RST, LORA_DIO1);

// Variables para manejo de comandos
volatile bool sendFlag = false;
String pendingCmd;

// Callback MQTT
void mqttCallback(char* topic, byte* payload, unsigned int len) {
  String cmd;
  for (unsigned int i = 0; i < len; i++) cmd += (char)payload[i];
  Serial.println("[MQTT] Comando recibido: " + cmd);
  pendingCmd = cmd;
  sendFlag = true;
}

void reconnectMQTT() {
  while (!mqtt.connected()) {
    Serial.print("[MQTT] Reconectando...");
    if (mqtt.connect("gateway_radiolib")) {
      Serial.println("OK");
      mqtt.subscribe(TOPIC_COMANDOS);
      Serial.println("[MQTT] Subscrito a " + String(TOPIC_COMANDOS));
    } else {
      Serial.print("ERR rc=");
      Serial.print(mqtt.state());
      Serial.println(" → retry en 3s");
      delay(3000);
    }
  }
}

void setup() {
    // Desactivar Brownout Detector durante inicialización
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  
  Serial.begin(115200);
  while (!Serial);

  // 1) Conectar WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("[WiFi] Conectando");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\n[WiFi] OK, IP: " + WiFi.localIP().toString());

  // 2) Configurar MQTT
  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt.setCallback(mqttCallback);
  reconnectMQTT();

  // 3) Inicializar LoRa con RadioLib (Clase C)
  SPI.begin(18, 19, 23, LORA_SS);  // SCK=18, MISO=19, MOSI=23, SS=5 (ESP32)
  
  Serial.println("[LoRa] Inicializando módulo...");
  int state = radio.begin(433.0, 125.0, 9, 5, 0xF3, 17);  // Freq=433MHz, BW=125kHz, SF=9, CR=4/5, SyncWord=0xF3, Power=17dBm
  
  if (state != RADIOLIB_ERR_NONE) {
    Serial.print("[LoRa] Error inicializando: ");
    Serial.println(state);
    while (true);  // Bloquea si hay error
  }
  
  radio.setRfSwitchPins(-1, -1);  // Opcional: si no usas control de antena
  radio.setPreambleLength(8);          // Longitud de preamble más larga
  radio.setCRC(true);                  // Forzar uso de CRC
  radio.setSyncWord(0xF3);             // Establecer palabra de sincronización

  Serial.println("[LoRa] Inicializado (Clase C)");



  Serial.println("[Sistema] Gateway listo");
}

void loop() {
  // 1) Mantener conexión MQTT
  if (!mqtt.connected()) reconnectMQTT();
  mqtt.loop();

  // 2) Enviar comandos pendientes por LoRa
  if (sendFlag) {
    sendFlag = false;
    
    // Verificar conexión MQTT antes de enviar
    if (!mqtt.connected()) {
        reconnectMQTT();
        if (!mqtt.connected()) {
            Serial.println("[ERROR] No se pudo conectar a MQTT, comando no enviado");
            return;
        }
    }

    Serial.println("[LoRa] Enviando comando: " + pendingCmd);
    
    // Sistema de reintentos mejorado
    int maxRetries = 3;
    for (int i = 0; i < maxRetries; i++) {
        int state = radio.transmit(pendingCmd);
        
        if (state == RADIOLIB_ERR_NONE) {
            Serial.println("[LoRa] Comando enviado con éxito");
            break;
        } else {
            Serial.print("[LoRa] Error en TX (intento ");
            Serial.print(i+1);
            Serial.print("): ");
            Serial.println(state);
            
            if (i == maxRetries - 1) {
                Serial.println("[ERROR] Fallo al enviar comando después de reintentos");
            }
            delay(500); // Espera entre reintentos
        }
    }
}

  // 3) Recibir datos de nodos LoRa y publicar a MQTT

String datosLoRa;
int state = radio.receive(datosLoRa);

if (state == RADIOLIB_ERR_NONE) {
    // Verificar que los datos son válidos antes de publicar
    if (datosLoRa.length() > 5 && datosLoRa.indexOf("{") >= 0) {
        Serial.println("[LoRa] Datos válidos recibidos: " + datosLoRa);
        
        // Publicar con QoS 1 para mayor confiabilidad
        if (mqtt.publish(TOPIC_DATOS, datosLoRa.c_str(), true)) {
            Serial.println("[MQTT] Datos publicados con QoS 1");
        } else {
            Serial.println("[ERROR] Fallo al publicar en MQTT");
        }
    } else {
        Serial.println("[LoRa] Datos recibidos no válidos");
    }
} else if (state != RADIOLIB_ERR_RX_TIMEOUT) {
    Serial.print("[LoRa] Error en recepción: ");
    Serial.println(state);
}

}
