#include "LDRSensor.h"

LDRSensor::LDRSensor(int pin) : _pin(pin) {}

void LDRSensor::begin() {
    pinMode(_pin, INPUT);
}

int LDRSensor::read() {
    return analogRead(_pin);
}

float LDRSensor::readLux() {
    // Ejemplo simple, debes calibrar según tu LDR y divisor de tensión
    int value = analogRead(_pin);
    float voltage = value * (3.3 / 4095.0); // Para ESP32 ADC 12 bits
    // Conversión ficticia, reemplaza con tu fórmula/calibración real
    float lux = 500 / voltage; 
    return lux;
}