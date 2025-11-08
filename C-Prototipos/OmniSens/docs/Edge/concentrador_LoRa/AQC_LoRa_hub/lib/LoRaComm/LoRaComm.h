#ifndef LORACOMM_H
#define LORACOMM_H

#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

class LoRaComm {
public:
    LoRaComm(long frequency = 433E6);
    bool begin();
    bool sendMessage(const String &msg);
    String receiveMessage();
    void setPins(int ss, int rst, int dio0); // Personalizar pines si se requiere

private:
    long _frequency;
    int _ss, _rst, _dio0;
    bool _customPins = false;
};

#endif
