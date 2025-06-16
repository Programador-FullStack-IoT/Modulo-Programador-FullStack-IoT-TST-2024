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
    void sendMessageSafely(const String &msg, unsigned long delayAfterMs); 
    String receiveMessage();
    void setPins(uint8_t ss, uint8_t rst, uint8_t dio0); // Personalizar pines si se requiere
    
    String getLastMessage();
    bool hasNewMessage();  

private:
    long _frequency;
    int _ss, _rst, _dio0;
    bool _customPins = false;
    static void onReceive(int packetSize);
    static volatile bool _messageAvailable;
    static String _incomingMessage;
};

#endif
