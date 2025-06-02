#include <Arduino.h>
#include <WiFi.h>
#include <Ticker.h>
#include "EnlaceLoraConcentrador.h"
#include "MqttManager.h"
#include "DatoJson.h"

// Configuración WiFi y MQTT
const char* WIFI_SSID = "Fibertel-WiFi032";
const char* WIFI_PASS = "vuelalto@67";
const char* MQTT_SERVER = "telecomunicaciones.ddns.net";
const int   MQTT_PORT = 2480;

// LoRa
#define LORA_CS   18
#define LORA_RST  14
#define LORA_INT  26

EnlaceLoraConcentrador lora(LORA_CS, LORA_RST, LORA_INT);
MqttManager mqtt(MQTT_SERVER, MQTT_PORT);
DatoJson jsonHelper;

// Timers
//Ticker loraTicker;
//Ticker mqttTicker;

// --- Función para publicar datos recibidos por LoRa ---
void onLoraJsonReceived(const String& json) {
    Serial.println("[LORA] Recibido JSON por LoRa:");
    Serial.println(json);
    String nodeId, token;
    
    // Extrae nodeId y token del JSON
    DynamicJsonDocument doc(512);
    DeserializationError error = deserializeJson(doc, json);
    if (error) {
        Serial.println("[LORA] JSON inválido");
        return;
    }
    nodeId = doc["id"] | "";
    token = doc["token"] | "";
    // Validar token aquí si lo deseas
    // Publicar en MQTT
    String topic = "omnisens/datos/" + nodeId;
    Serial.print("[MQTT] Publicando datos en el topic: ");
    Serial.println(topic);
    mqtt.publish(topic.c_str(), json.c_str());
    Serial.print("[MQTT] Publicado en ");
    Serial.print(topic);
    Serial.print(": ");
    Serial.println(json);
    // Enviar ACK por LoRa
    Serial.print("[LORA] Enviando ACK a nodo: ");
    Serial.println(nodeId);
    lora.sendAck(nodeId);
}

// --- Función para retransmitir órdenes de MQTT a LoRa ---
void onMqttOrderReceived(char* topic, byte* payload, unsigned int length) {
    Serial.print("[MQTT] Orden recibida en topic: ");
    Serial.println(topic);
    String json((char*)payload, length);
    // Aquí podrías validar destinatario, token, etc.
    Serial.print("[LORA] Retransmitiendo orden por LoRa: ");
    Serial.println(json);
    lora.sendJson(json);
    Serial.print("[LORA] Orden retransmitida: ");
    //Serial.println(json);
}

void setup() {
    Serial.begin(115200);
    Serial.println("[Sistema] Inicializando concentrador...");

    Serial.print("[WiFi] Conectando a ");
    Serial.println(WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n[WiFi] Conectado correctamente");

    Serial.println("[LoRa] Inicializando módulo LoRa...");
    lora.begin(433.0);
    lora.setDebug(true);
    lora.onJsonReceived(onLoraJsonReceived);
    Serial.println("[LoRa] Módulo LoRa listo");

    Serial.println("[MQTT] Inicializando cliente MQTT...");
    mqtt.setDebug(true);
    mqtt.onOrderReceived(onMqttOrderReceived);
    mqtt.connect();
    Serial.println("[MQTT] Cliente MQTT listo");

    // Timers periódicos
   // loraTicker.attach_ms(50, [](){ lora.loop(); });
   // mqttTicker.attach_ms(100, [](){ mqtt.loop(); });

    Serial.println("[Sistema] Concentrador inicializado y en funcionamiento");
}

void loop() {
    lora.loop();
    mqtt.loop();
    delay(10); // Permite que el sistema respire y evita el watchdog
}