#include "WS2812Controller.h"

WS2812Controller::WS2812Controller(int pin) : strip(1, pin, NEO_GRB + NEO_KHZ800) {}

void WS2812Controller::begin() {
    strip.begin();
    strip.show();
}

void WS2812Controller::updateLED(int aqi) {
    uint32_t color;
    if (aqi <= 50) {
        color = strip.Color(0, 255, 0); // Verde
    } else if (aqi <= 100) {
        color = strip.Color(255, 255, 0); // Amarillo
    } else {
        color = strip.Color(255, 0, 0); // Rojo
    }
    strip.setPixelColor(0, color);
    strip.show();
}
