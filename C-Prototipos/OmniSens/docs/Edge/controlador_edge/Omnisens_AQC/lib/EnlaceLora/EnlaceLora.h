#ifndef ENLACELORA_H
#define ENLACELORA_H

#include <Arduino.h>
#include <RH_RF95.h>
#include <functional>

class EnlaceLora {
public:
    EnlaceLora(const String& nodeId, const String& token, uint8_t csPin, uint8_t rstPin, uint8_t intPin);

    bool begin(float freqMHz = 433.0);
    void setDebug(bool enable);
    void sendJson(const String& json);
    void loop();
    void onJsonReceived(std::function<void(const String&)> callback);

private:
    String _nodeId;
    String _token;
    uint8_t _csPin, _rstPin, _intPin;
    bool _debug;
    RH_RF95* _rf95;
    std::function<void(const String&)> _jsonCallback;
    unsigned long _lastKeepAlive;
    void sendKeepAlive();
    void sendAck(uint8_t packetId);
    void processReceived();
};

#endif