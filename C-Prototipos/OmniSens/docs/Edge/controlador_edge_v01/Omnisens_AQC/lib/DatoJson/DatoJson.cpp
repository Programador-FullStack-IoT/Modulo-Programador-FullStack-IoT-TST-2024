#include "DatoJson.h"

DatoJson::DatoJson() {}

String DatoJson::generarJson(
    const String& id, uint32_t timestamp, const String& token,
    float mq135, float ahtTemp, float ahtHum, float bmpTemp, float bmpPres,
    float luz, float uv, uint8_t pwm, bool rele1, bool rele2, bool alarmaGas, uint8_t codigoAlarma
) {
    DynamicJsonDocument doc(512);
    doc.clear();
    doc["id"] = id;
    doc["timestamp"] = timestamp;
    doc["token"] = token;
    doc["sensores"]["mq135"] = mq135;
    doc["sensores"]["aht_temp"] = ahtTemp;
    doc["sensores"]["aht_hum"] = ahtHum;
    doc["sensores"]["bmp_temp"] = bmpTemp;
    doc["sensores"]["bmp_pres"] = bmpPres;
    doc["sensores"]["luz"] = luz;
    doc["sensores"]["uv"] = uv;
    doc["actuadores"]["pwm"] = pwm;
    doc["actuadores"]["rele1"] = rele1;
    doc["actuadores"]["rele2"] = rele2;
    doc["alarmas"]["gas"] = alarmaGas;
    doc["alarmas"]["codigoAlarma"] = codigoAlarma;
    String output;
    serializeJson(doc, output);
    return output;
}

String DatoJson::generarJsonPretty(
    const String& id, uint32_t timestamp, const String& token,
    float mq135, float ahtTemp, float ahtHum, float bmpTemp, float bmpPres,
    float luz, float uv, uint8_t pwm, bool rele1, bool rele2, bool alarmaGas, uint8_t codigoAlarma
) {
    DynamicJsonDocument doc(512);
    doc.clear();
    doc["id"] = id;
    doc["timestamp"] = timestamp;
    doc["token"] = token;
    doc["sensores"]["mq135"] = mq135;
    doc["sensores"]["aht_temp"] = ahtTemp;
    doc["sensores"]["aht_hum"] = ahtHum;
    doc["sensores"]["bmp_temp"] = bmpTemp;
    doc["sensores"]["bmp_pres"] = bmpPres;
    doc["sensores"]["luz"] = luz;
    doc["sensores"]["uv"] = uv;
    doc["actuadores"]["pwm"] = pwm;
    doc["actuadores"]["rele1"] = rele1;
    doc["actuadores"]["rele2"] = rele2;
    doc["alarmas"]["gas"] = alarmaGas;
    doc["alarmas"]["codigoAlarma"] = codigoAlarma;
    String output;
    serializeJsonPretty(doc, output);
    return output;
}

bool DatoJson::procesarOrdenes(const String& json, const String& id, uint8_t& pwm, bool& rele1, bool& rele2, uint8_t& codigoAlarma) {
    DynamicJsonDocument doc(512); 
    doc.clear();
    DeserializationError error = deserializeJson(doc, json);
    if (error) return false;

    if (doc.containsKey("o") && doc["id"] == id) {
        if (doc["o"].containsKey("pwm")) {
            pwm = doc["o"]["pwm"];
        }
        if (doc["o"].containsKey("r1")) {
            rele1 = doc["o"]["r1"];
        }
        if (doc["o"].containsKey("r2")) {
            rele2 = doc["o"]["r2"];
        }
        if (doc["o"].containsKey("cA")) {
            codigoAlarma = doc["o"]["cA"];
        }
        return true;
    }
    return false;
}