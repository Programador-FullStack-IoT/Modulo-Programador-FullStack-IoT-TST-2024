#ifndef MQ135SENSOR_H
#define MQ135SENSOR_H

#include <Arduino.h>
#include <MQ135.h>

class MQ135Sensor {
private:
    MQ135 mq135;
    const int NUM_READINGS = 10;
    float mq135Readings[10];
    int mq135ReadIndex = 0;
    float mq135Total = 0;
    float mq135Average = 0;

public:
    MQ135Sensor(int pin);
    void begin();
    float readFilteredData();
};

#endif
