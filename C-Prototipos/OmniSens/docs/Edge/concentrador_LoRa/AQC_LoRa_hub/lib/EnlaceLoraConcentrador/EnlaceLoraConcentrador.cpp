#include "EnlaceLoraConcentrador.h"
#include <ArduinoJson.h>

EnlaceLoraConcentrador::EnlaceLoraConcentrador(uint8_t csPin, uint8_t rstPin, uint8_t intPin)
    : _csPin(csPin), _rstPin(rstPin), _intPin(intPin), _debug(false), _rf95(nullptr) {}

bool EnlaceLoraConcentrador::begin(float freqMHz) {
    pinMode(_rstPin, OUTPUT);
    digitalWrite(_rstPin, HIGH); delay(10);
    digitalWrite(_rstPin, LOW); delay(10);
    digitalWrite(_rstPin, HIGH); delay(10);

    _rf95 = new RH_RF95(_csPin, _intPin);
    if (!_rf95->init()) {
        if (_debug) Serial.println("[LoRa] Fallo al inicializar RF95");
        return false;
    }
    if (!_rf95->setFrequency(freqMHz)) {
        if (_debug) Serial.println("[LoRa] Frecuencia inválida");
        return false;
    }
    _rf95->setTxPower(13, false);
    if (_debug) Serial.println("[LoRa] Inicializado correctamente");
    return true;
}

void EnlaceLoraConcentrador::setDebug(bool enable) {
    _debug = enable;
}

void EnlaceLoraConcentrador::onJsonReceived(std::function<void(const String&)> callback) {
    _jsonCallback = callback;
}

void EnlaceLoraConcentrador::setTokenMap(const std::map<String, String>& tokens) {
    _tokenMap = tokens;
}

void EnlaceLoraConcentrador::loop() {
    processReceived();
}

void EnlaceLoraConcentrador::sendJson(const String& json) {
    if (_rf95) {
        uint8_t data[RH_RF95_MAX_MESSAGE_LEN];
        size_t len = json.length();
        if (len > RH_RF95_MAX_MESSAGE_LEN - 1) len = RH_RF95_MAX_MESSAGE_LEN - 1;
        memcpy(data, json.c_str(), len);
        data[len] = 0;
        _rf95->send(data, len + 1);
        _rf95->waitPacketSent();
        if (_debug) {
            Serial.print("[LoRa] Enviado JSON: ");
            Serial.println(json);
        }
    }
}

void EnlaceLoraConcentrador::sendAck(const String& nodeId) {
    String ack = "{\"id\":\"" + nodeId + "\",\"ack\":1}";
    sendJson(ack);
    if (_debug) Serial.println("[LoRa] ACK enviado");
}

bool EnlaceLoraConcentrador::validateToken(const String& nodeId, const String& token) {
    auto it = _tokenMap.find(nodeId);
    if (it != _tokenMap.end() && it->second == token) return true;
    if (_debug) Serial.println("[LoRa] Token inválido para nodeId: " + nodeId);
    return false;
}

void EnlaceLoraConcentrador::processReceived() {
    if (_rf95 && _rf95->available()) {
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
        if (_rf95->recv(buf, &len)) {
            String msg = String((char*)buf);
            if (_debug) {
                Serial.print("[LoRa] Recibido: ");
                Serial.println(msg);
            }
            // Validar token antes de pasar al callback
            DynamicJsonDocument doc(512);
            DeserializationError error = deserializeJson(doc, msg);
            if (!error) {
                String nodeId = doc["id"] | "";
                String token = doc["token"] | "";
                if (validateToken(nodeId, token)) {
                    if (_jsonCallback) _jsonCallback(msg);
                }
            }
        }
    }
}