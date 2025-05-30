#include "AHT25Sensor.h"

AHT25Sensor::AHT25Sensor() {}

bool AHT25Sensor::begin() {
    if (!aht.begin()) {
        Serial.println("Error: No se encontró el sensor AHT25. Verifica las conexiones!");
        return false;
        //while (1);
    } 
    else {

        return true;
    }
}

bool AHT25Sensor::readData(float &temperature, float &humidity) {
    sensors_event_t humidity_event, temp_event;
    aht.getEvent(&humidity_event, &temp_event);

    // Filtrado de datos erróneos
    if (humidity_event.relative_humidity < 0 || humidity_event.relative_humidity > 100 ||
        temp_event.temperature < -40 || temp_event.temperature > 80) {
        return false;
    }

    temperature = temp_event.temperature;
    humidity = humidity_event.relative_humidity;
    return true;
}