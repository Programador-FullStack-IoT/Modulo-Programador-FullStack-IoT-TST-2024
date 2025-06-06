#ifndef DATOJSON_H
#define DATOJSON_H
#include <Arduino.h>
#include <ArduinoJson.h>


class DatoJson {
public:
    DatoJson();
    String generarJson(
        float mq135, float ahtTemp, float ahtHum, float bmpTemp, float bmpPres,
        float luz, float uv, uint8_t pwm, bool rele1, bool rele2, bool alarmaGas, uint8_t codigoAlarma
    );
    bool procesarOrdenes(const String& json, uint8_t& pwm, bool& rele1, bool& rele2, uint8_t& codigoAlarma);

private:
    StaticJsonDocument<512> doc;
};

#endif