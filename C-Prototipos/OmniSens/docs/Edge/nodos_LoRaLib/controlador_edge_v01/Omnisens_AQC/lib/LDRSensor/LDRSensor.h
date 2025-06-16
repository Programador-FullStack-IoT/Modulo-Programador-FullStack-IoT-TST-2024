#ifndef LDRSENSOR_H
#define LDRSENSOR_H

#include <Arduino.h>

class LDRSensor {
public:
    LDRSensor(int pin);
    void begin();
    int read();
    float readLux(); // Opcional, requiere calibración
private:
    int _pin;
};

#endif