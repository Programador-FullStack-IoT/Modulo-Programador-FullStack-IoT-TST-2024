#ifndef SALIDAPWM_H
#define SALIDAPWM_H

#include <Arduino.h>

class SalidaPWM {
public:
    SalidaPWM(int pin);
    void begin();
    void comandoPWM(uint8_t porcentaje); // porcentaje: 0-100
    uint8_t getPWM() const;
private:
    int _pin;
    uint8_t _duty; // Último valor de duty cycle (0-100)
};

#endif