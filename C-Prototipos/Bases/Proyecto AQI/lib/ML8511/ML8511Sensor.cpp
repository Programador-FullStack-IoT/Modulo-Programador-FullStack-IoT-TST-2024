#include "ML8511Sensor.h"

ML8511Sensor::ML8511Sensor() {}

void ML8511Sensor::begin() {
    // No se necesita inicialización específica para ML8511
}

float ML8511Sensor::leerUV() {
    int uvAnalogValue = analogRead(UV_PIN);
    // Convertir valor analógico en índice UV (esto es un ejemplo simple)
    float uvVoltage = (uvAnalogValue / 4096.0) * 3.3;
    return uvVoltage * 10.0; // Escalado a un índice UV (simplificado)
}
