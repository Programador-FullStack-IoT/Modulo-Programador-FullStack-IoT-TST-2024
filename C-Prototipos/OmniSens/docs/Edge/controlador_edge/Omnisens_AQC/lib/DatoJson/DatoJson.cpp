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

    if (doc.containsKey("ordenes") && doc[id] == id) {
        if (doc["ordenes"].containsKey("pwm")) {
            pwm = doc["ordenes"]["pwm"];
        }
        if (doc["ordenes"].containsKey("rele1")) {
            rele1 = doc["ordenes"]["rele1"];
        }
        if (doc["ordenes"].containsKey("rele2")) {
            rele2 = doc["ordenes"]["rele2"];
        }
        if (doc["ordenes"].containsKey("codigoAlarma")) {
            codigoAlarma = doc["ordenes"]["codigoAlarma"];
        }
        return true;
    }
    return false;
}