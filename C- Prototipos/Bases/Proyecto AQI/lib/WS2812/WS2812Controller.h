#ifndef WS2812CONTROLLER_H
#define WS2812CONTROLLER_H

#include <Adafruit_NeoPixel.h>

class WS2812Controller {
private:
    Adafruit_NeoPixel strip;

public:
    WS2812Controller(int pin);
    void begin();
    void updateLED(int aqi);
};

#endif
