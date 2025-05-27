#ifndef BH1750SENSOR_H
#define BH1750SENSOR_H

#include <Wire.h>
#include <BH1750.h>

class BH1750Sensor {
private:
    BH1750 lightMeter;

public:
    BH1750Sensor();
    void begin();
    float leerLux();
};

#endif
