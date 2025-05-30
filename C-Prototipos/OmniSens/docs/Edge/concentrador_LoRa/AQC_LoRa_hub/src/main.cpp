#include <Arduino.h>
#include <WiFi.h>
#include <Ticker.h>
#include "EnlaceLoraConcentrador.h"
#include "MqttManager.h"
#include "DatoJson.h"

// Configuración WiFi y MQTT
const char* WIFI_SSID = "TU_SSID";
const char* WIFI_PASS = "TU_PASS";
const char* MQTT_SERVER = "broker.remoto.com";
const int   MQTT_PORT = 1883;

// LoRa
#define LORA_CS   18
#define LORA_RST  14
#define LORA_INT  26

EnlaceLoraConcentrador lora(LORA_CS, LORA_RST, LORA_INT);
MqttManager mqtt(MQTT_SERVER, MQTT_PORT);
DatoJson jsonHelper;

// Timers
Ticker loraTicker;
Ticker mqttTicker;

// --- Función para publicar datos recibidos por LoRa ---
void onLoraJsonReceived(const String& json) {
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
    mqtt.publish(topic.c_str(), json.c_str());
    Serial.print("[MQTT] Publicado en ");
    Serial.print(topic);
    Serial.print(": ");
    Serial.println(json);
    // Enviar ACK por LoRa
    lora.sendAck(nodeId);
}

// --- Función para retransmitir órdenes de MQTT a LoRa ---
void onMqttOrderReceived(char* topic, byte* payload, unsigned int length) {
    String json((char*)payload, length);
    // Aquí podrías validar destinatario, token, etc.
    lora.sendJson(json);
    Serial.print("[LORA] Orden retransmitida: ");
    Serial.println(json);
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\n[WiFi] Conectado");

    lora.begin(433.0);
    lora.setDebug(true);
    lora.onJsonReceived(onLoraJsonReceived);

    mqtt.setDebug(true);
    mqtt.onOrderReceived(onMqttOrderReceived);
    mqtt.connect();

    // Timers periódicos
    loraTicker.attach_ms(50, [](){ lora.loop(); });
    mqttTicker.attach_ms(100, [](){ mqtt.loop(); });

    Serial.println("[Sistema] Concentrador inicializado");
}

void loop() {
    // Nada, todo por timers
}