#include "Baliza_con.h"

Baliza_con::Baliza_con(int pin) : strip(1, pin, NEO_GRB + NEO_KHZ800) {}

void Baliza_con::begin() {
    strip.begin();
    strip.show();
}

void Baliza_con::updateLED(int aqi) {
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
