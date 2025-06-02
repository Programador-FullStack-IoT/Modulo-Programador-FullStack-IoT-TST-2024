#ifndef DATOJSON_H
#define DATOJSON_H
#include <Arduino.h>
#include <ArduinoJson.h>


class DatoJson {
public:
    DatoJson();
    String generarJson(
        const String& id, uint32_t timestamp, const String& token,
        float mq135, float ahtTemp, float ahtHum, float bmpTemp, float bmpPres,
        float luz, float uv, uint8_t pwm, bool rele1, bool rele2, bool alarmaGas, uint8_t codigoAlarma
    );
    String generarJsonPretty(
        const String& id, uint32_t timestamp, const String& token,
        float mq135, float ahtTemp, float ahtHum, float bmpTemp, float bmpPres,
        float luz, float uv, uint8_t pwm, bool rele1, bool rele2, bool alarmaGas, uint8_t codigoAlarma
    );
    
    bool procesarOrdenes(const String& json, const String& id, uint8_t& pwm, bool& rele1, bool& rele2, uint8_t& codigoAlarma);
};

#endif