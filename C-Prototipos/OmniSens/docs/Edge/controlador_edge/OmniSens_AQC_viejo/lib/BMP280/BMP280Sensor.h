#ifndef BMP280SENSOR_H
#define BMP280SENSOR_H

#include <Arduino.h>
#include <Adafruit_BMP280.h>

class BMP280Sensor {
private:
    Adafruit_BMP280 bmp;

public:
    BMP280Sensor();
    void begin();
    bool readData(float &temperature, float &pressure);
};

#endif
