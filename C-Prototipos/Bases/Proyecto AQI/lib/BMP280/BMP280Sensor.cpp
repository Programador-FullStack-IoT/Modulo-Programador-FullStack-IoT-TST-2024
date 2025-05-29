#include "BMP280Sensor.h"

BMP280Sensor::BMP280Sensor() {}

void BMP280Sensor::begin() {
    if (!bmp.begin(0x76)) { // Dirección I2C puede ser 0x76 o 0x77
        Serial.println("Error: No se encontró el sensor BMP280. Verifica las conexiones!");
        while (1);
    }
}

bool BMP280Sensor::readData(float &temperature, float &pressure) {
    temperature = bmp.readTemperature();
    pressure = bmp.readPressure() / 100.0F; // Convertir a hPa

    // Filtrado de datos erróneos
    if (temperature < -40 || temperature > 80 ||
        pressure < 300 || pressure > 1100) { // hPa
        return false;
    }

    return true;
}
