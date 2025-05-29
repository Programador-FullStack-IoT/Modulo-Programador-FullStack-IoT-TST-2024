#include "SalidasRele.h"

SalidasRele::SalidasRele(int pin1, int pin2)
    : _pin1(pin1), _pin2(pin2), _estado1(false), _estado2(false) {}

void SalidasRele::begin() {
    pinMode(_pin1, OUTPUT);
    pinMode(_pin2, OUTPUT);
    setRele1(false);
    setRele2(false);
}

void SalidasRele::setRele1(bool estado) {
    _estado1 = estado;
    digitalWrite(_pin1, estado ? HIGH : LOW);
}

void SalidasRele::setRele2(bool estado) {
    _estado2 = estado;
    digitalWrite(_pin2, estado ? HIGH : LOW);
}

void SalidasRele::toggleRele1() {
    setRele1(!_estado1);
}

void SalidasRele::toggleRele2() {
    setRele2(!_estado2);
}

bool SalidasRele::getRele1() const {
    return _estado1;
}

bool SalidasRele::getRele2() const {
    return _estado2;
}