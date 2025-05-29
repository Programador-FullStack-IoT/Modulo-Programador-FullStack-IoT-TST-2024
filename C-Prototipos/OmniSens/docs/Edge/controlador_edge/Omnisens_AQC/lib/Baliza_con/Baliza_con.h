#ifndef BALIZA_CON_H
#define BALIZA_CON_H

#include <Adafruit_NeoPixel.h>

class Baliza_con {
private:
    Adafruit_NeoPixel strip;

public:
    Baliza_con(int pin);
    void begin();
    void updateLED(int aqi);
};

#endif
