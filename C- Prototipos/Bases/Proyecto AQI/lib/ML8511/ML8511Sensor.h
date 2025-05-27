#ifndef ML8511SENSOR_H
#define ML8511SENSOR_H

#include <Arduino.h>

class ML8511Sensor {
private:
    const int UV_PIN = 33; // Pin analógico para el ML8511

public:
    ML8511Sensor();
    void begin();
    float leerUV();
};

#endif
