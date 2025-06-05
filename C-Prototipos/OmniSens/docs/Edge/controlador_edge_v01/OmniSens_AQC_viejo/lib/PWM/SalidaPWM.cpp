#include "SalidaPWM.h"

SalidaPWM::SalidaPWM(int pin) : _pin(pin), _duty(0) {}

void SalidaPWM::begin() {
    ledcAttachPin(_pin, 0); // Canal 0
    ledcSetup(0, 5000, 8);  // Canal 0, 5kHz, 8 bits de resolución
    comandoPWM(0); // Inicializa en 0%
}

void SalidaPWM::comandoPWM(uint8_t porcentaje) {
    if (porcentaje > 100) porcentaje = 100;
    _duty = porcentaje;
    uint8_t valorPWM = map(_duty, 0, 100, 0, 255);
    ledcWrite(0, valorPWM);
}

uint8_t SalidaPWM::getPWM() const {
    return _duty;
}