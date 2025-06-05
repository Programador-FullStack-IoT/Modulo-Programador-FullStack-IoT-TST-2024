#include "LoRaComm.h"

LoRaComm::LoRaComm(long frequency) : _frequency(frequency) {
    //Serial.println("seteando pines LoRa");
    _ss = 5;
    _rst = 14;
    _dio0 = 26;
}

void LoRaComm::setPins(int ss, int rst, int dio0) {
    Serial.println("asignando pines LoRa");
    _ss = ss;
    _rst = rst;
    _dio0 = dio0;
    _customPins = true;
}

bool LoRaComm::begin() {
    Serial.println("Configurando LoRa");
    if (_customPins)
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

bool LoRaComm::sendMessage(const String &msg) {
    LoRa.beginPacket();
    LoRa.print(msg);
    LoRa.endPacket();
    return true;
}

String LoRaComm::receiveMessage() {
    String incoming = "";
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        while (LoRa.available()) {
            incoming += (char)LoRa.read();
        }
    }
    return incoming;
}
