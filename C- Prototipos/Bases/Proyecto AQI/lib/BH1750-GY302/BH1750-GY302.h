#ifndef BH1750_SENSOR_H
#define BH1750_SENSOR_H

#include <Wire.h>

#define BH1750_I2C_ADDRESS 0x23  // Dirección I2C por defecto del BH1750

class BH1750_Sensor {
public:
    BH1750_Sensor();
    void begin();
    float readLightLevel();  // Simula la lectura de la luz en lux

private:
    void writeCommand(uint8_t cmd);
    uint16_t readRawData();
};

#endif
