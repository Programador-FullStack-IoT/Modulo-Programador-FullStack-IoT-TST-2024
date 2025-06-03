#include "EnlaceLora.h"

EnlaceLora::EnlaceLora(const String& nodeId, const String& token, uint8_t csPin, uint8_t rstPin, uint8_t intPin)
    : _nodeId(nodeId), _token(token), _csPin(csPin), _rstPin(rstPin), _intPin(intPin), _debug(false), _rf95(nullptr), _lastKeepAlive(0) {}

bool EnlaceLora::begin(float freqMHz) {
    pinMode(_rstPin, OUTPUT);
    digitalWrite(_rstPin, HIGH);
    delay(10);
    digitalWrite(_rstPin, LOW);
    delay(10);
    digitalWrite(_rstPin, HIGH);
    delay(10);

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

void EnlaceLora::setDebug(bool enable) {
    _debug = enable;
}

void EnlaceLora::sendJson(const String& json) {
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

void EnlaceLora::loop() {
    // Keep alive cada 1 segundo
    if (millis() - _lastKeepAlive > 1000) {
        sendKeepAlive();
        _lastKeepAlive = millis();
    }
    processReceived();
}

void EnlaceLora::onJsonReceived(std::function<void(const String&)> callback) {
    _jsonCallback = callback;
}

void EnlaceLora::sendKeepAlive() {
    String keepAlive = "{\"id\":\"" + _nodeId + "\",\"token\":\"" + _token + "\",\"keepalive\":1}";
    sendJson(keepAlive);
    if (_debug) Serial.println("[LoRa] KeepAlive enviado");
}

void EnlaceLora::sendAck(uint8_t packetId) {
    String ack = "{\"id\":\"" + _nodeId + "\",\"token\":\"" + _token + "\",\"ack\":" + String(packetId) + "}";
    sendJson(ack);
    if (_debug) Serial.println("[LoRa] ACK enviado");
}

void EnlaceLora::processReceived() {
    if (_rf95 && _rf95->available()) {
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);
        if (_rf95->recv(buf, &len)) {
            String msg = String((char*)buf);
            if (_debug) {
                Serial.print("[LoRa] Recibido: ");
                Serial.println(msg);
            }
            // Llama al callback si está definido
            if (_jsonCallback) {
                _jsonCallback(msg);
            }
        }
    }
}