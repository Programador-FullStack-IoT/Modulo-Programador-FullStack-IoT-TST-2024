// LoRaComm.cpp
#include "LoRaComm.h"
#include <SPI.h>

LoRaComm::LoRaComm(long frequency) : _frequency(frequency), _ss(5), _rst(14), _dio0(26) {}

void LoRaComm::setPins(uint8_t ss, uint8_t rst, uint8_t dio0) {
    _ss = ss;
    _rst = rst;
    _dio0 = dio0;
}

bool LoRaComm::begin() {
    LoRa.setPins(_ss, _rst, _dio0);
    if (!LoRa.begin(_frequency)) {
        Serial.println("[LoRaComm] Error al iniciar LoRa");
        return false;
    }
    LoRa.setSpreadingFactor(12);
    LoRa.setSignalBandwidth(125E3);
    LoRa.setCodingRate4(5);
    Serial.println("[LoRaComm] LoRa iniciado");
    return true;
}


bool LoRaComm::sendMessage(const String& message) {
    LoRa.beginPacket();
    LoRa.print(message);
    LoRa.endPacket(true);
    LoRa.receive(); // Asegurar que LoRa vuelve a modo escucha
    Serial.println("[LoRaComm] Mensaje enviado");
    return true;
}

void LoRaComm::sendMessageSafely(const String& message, unsigned long delayAfterMs) {
    LoRa.beginPacket();
    LoRa.print(message);
    LoRa.endPacket(true);
    Serial.println("[LoRaComm] Mensaje enviado de forma segura");
    delay(delayAfterMs);  // Permitir que vuelva a modo escucha
}

String LoRaComm::receiveMessage() {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        Serial.print("[LoRaComm] Paquete recibido. Tamaño: ");
        Serial.println(packetSize);

        String incoming = "";
        while (LoRa.available()) {
            char c = (char)LoRa.read();
            Serial.print(c); // Debug crudo
            incoming += c;
        }
        Serial.println();
        return incoming;
    }
    return "";
}
