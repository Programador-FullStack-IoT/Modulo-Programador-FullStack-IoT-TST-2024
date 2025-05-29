#include "BH1750Sensor.h"

BH1750Sensor::BH1750Sensor() : lightMeter() {}

void BH1750Sensor::begin() {
    lightMeter.begin();
}

float BH1750Sensor::leerLux() {
    return lightMeter.readLightLevel();
}
