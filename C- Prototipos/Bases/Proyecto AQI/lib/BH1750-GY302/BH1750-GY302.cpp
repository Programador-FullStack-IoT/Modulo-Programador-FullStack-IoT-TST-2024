#include "BH1750_Sensor.h"

BH1750_Sensor::BH1750_Sensor() {
    // Constructor vacío
}

void BH1750_Sensor::begin() {
    Wire.begin();  // Iniciar I2C
    writeCommand(0x10);  // Encender el sensor en modo de alta resolución
}

void BH1750_Sensor::writeCommand(uint8_t cmd) {
    Wire.beginTransmission(BH1750_I2C_ADDRESS);
    Wire.write(cmd);
    Wire.endTransmission();
}

uint16_t BH1750_Sensor::readRawData() {
    uint16_t level = 0;

    Wire.requestFrom(BH1750_I2C_ADDRESS, 2);
    if (Wire.available() == 2) {
        level = Wire.read();
        level <<= 8;
        level |= Wire.read();
    }

    return level;
}

float BH1750_Sensor::readLightLevel() {
    uint16_t rawData = readRawData();
    // Convierte los datos crudos en lux (1 unidad equivale a 1 lux para el BH1750)
    return rawData / 1.2;  // Simulación aproximada del factor de conversión
}
