#ifndef AHT25SENSOR_H
#define AHT25SENSOR_H

#include <Arduino.h>
#include <Adafruit_AHTX0.h>

class AHT25Sensor {
private:
    Adafruit_AHTX0 aht;

public:
    AHT25Sensor();
    void begin();
    bool readData(float &temperature, float &humidity);
};

#endif
