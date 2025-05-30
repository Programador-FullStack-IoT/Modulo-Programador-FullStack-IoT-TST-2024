#include "MqttManager.h"

MqttManager* MqttManager::instance = nullptr;

MqttManager::MqttManager(const char* server, uint16_t port)
    : _server(server), _port(port), _mqttClient(_wifiClient), _debug(false), _lastReconnectAttempt(0) {
    instance = this;
    _mqttClient.setServer(_server, _port);
    _mqttClient.setCallback(_mqttCallback);
}

void MqttManager::setDebug(bool enable) {
    _debug = enable;
}

void MqttManager::connect() {
    while (!_mqttClient.connected()) {
        if (_debug) Serial.print("[MQTT] Intentando conectar...");
        String clientId = "OmniConcentrador-" + String(random(0xffff), HEX);
        if (_mqttClient.connect(clientId.c_str())) {
            if (_debug) Serial.println("conectado!");
            // Suscribirse a topic de órdenes comodín
            subscribe("omnisens/cmd/+");
        } else {
            if (_debug) {
                Serial.print("falló, rc=");
                Serial.print(_mqttClient.state());
                Serial.println(" intentando de nuevo en 2s");
            }
            delay(2000);
        }
    }
}

void MqttManager::loop() {
    if (!_mqttClient.connected()) {
        unsigned long now = millis();
        if (now - _lastReconnectAttempt > 5000) {
            _lastReconnectAttempt = now;
            connect();
        }
    }
    _mqttClient.loop();
}

void MqttManager::publish(const char* topic, const char* payload) {
    if (_debug) {
        Serial.print("[MQTT] Publicando en ");
        Serial.print(topic);
        Serial.print(": ");
        Serial.println(payload);
    }
    _mqttClient.publish(topic, payload);
}

void MqttManager::subscribe(const char* topic) {
    _mqttClient.subscribe(topic);
    if (_debug) {
        Serial.print("[MQTT] Suscrito a ");
        Serial.println(topic);
    }
}

void MqttManager::onOrderReceived(std::function<void(char*, byte*, unsigned int)> callback) {
    _orderCallback = callback;
}

void MqttManager::_mqttCallback(char* topic, byte* payload, unsigned int length) {
    if (instance && instance->_orderCallback) {
        instance->_orderCallback(topic, payload, length);
    }
}