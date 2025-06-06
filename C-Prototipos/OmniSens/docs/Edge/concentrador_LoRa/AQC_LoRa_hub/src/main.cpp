#include <Arduino.h>
#include <WiFi.h>
#include "PubSubClient.h"
#include <SPI.h>
#include "LoRaComm.h"

//#include "DatoJson.h"

// Configuración WiFi
const char* WIFI_SSID = "Fibertel-WiFi032";
const char* WIFI_PASS = "vuelalto@67";

// Configuración MQTT
const char* MQTT_SERVER = "telecomunicaciones.ddns.net";
const int   MQTT_PORT = 2480;
const char* MQTT_TOPIC = "omnisens/datos/AQC_001"; // Topic base para publicar datos

// LoRa
#define LORA_CS   5
#define LORA_RST  14
#define LORA_INT  26

LoRaComm lora(433E6);


WiFiClient espClient;

PubSubClient mqtt(espClient);



void setup() {
    Serial.begin(115200);
    delay(1000); // Espera para estabilizar la conexión serial
    Serial.println("\n\n[Sistema] Nodo concentrador LoRa + MQTT");
    Serial.println("[Sistema] Inicializando concentrador...");


    // Iniciar WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print("[WiFi] Conectando");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n[WiFi] Conectado a " + String(WIFI_SSID));
    Serial.print("[WiFi] IP local: ");
    Serial.println(WiFi.localIP());
    delay(1000); // Espera para estabilizar la conexión

    // Iniciar MQTT
    mqtt.setServer(MQTT_SERVER, MQTT_PORT);

    // Iniciar SPI + LoRa
    SPI.begin(18, 19, 23, LORA_CS);
    lora.setPins(LORA_CS, LORA_RST, LORA_INT);
    if (lora.begin()) {
        Serial.println("[LoRa] Inicializado");
    } else {
        Serial.println("[LoRa] ERROR al iniciar");
    }
    // Timers periódicos
   // loraTicker.attach_ms(50, [](){ lora.loop(); });
   // mqttTicker.attach_ms(100, [](){ mqtt.loop(); });

    Serial.println("[Sistema] Concentrador inicializado y en funcionamiento");
}

void loop() {
  if (!mqtt.connected()) {
    Serial.print("[MQTT] Reconectando...");
    while (!mqtt.connected()) {
      if (mqtt.connect("control_mqtt")) {
        Serial.println(" conectado.");
      } else {
        Serial.print(" fallo (rc=");
        Serial.print(mqtt.state());
        Serial.println("), reintentando en 5s");
        delay(5000);
      }
    }
  }

  mqtt.loop(); // mantiene conexión viva

  // Escuchar LoRa
  String mensaje = lora.receiveMessage();
  if (mensaje.length() > 0) {
    Serial.println("[LoRa] Mensaje recibido: " + mensaje);

    if (mqtt.publish(MQTT_TOPIC, mensaje.c_str())) {
      Serial.println("[MQTT] Publicado en " + String(MQTT_TOPIC));
    } else {
      Serial.println("[MQTT] Fallo al publicar");
    }
  }
}