#include "SensorData.h"

SensorData::SensorData(String id, MQ135Sensor* mq, BMP280Sensor* bmp, AHT25Sensor* aht,
                       LDRSensor* ldr, SalidasRele* rele, SalidaPWM* pwm)
    : _id(id), _mq(mq), _bmp(bmp), _aht(aht), _ldr(ldr), _rele(rele), _pwm(pwm) {}

String SensorData::toJSON() {
    float gas = _mq->readFilteredData();

    float temp_bmp = 0.0, presion = 0.0;
    _bmp->readData(temp_bmp, presion);

    float temp_aht = 0.0, humedad = 0.0;
    _aht->readData(temp_aht, humedad);

    int luz = _ldr->read();

    int rele1 = _rele->getRele1() ? 1 : 0;
    int rele2 = _rele->getRele2() ? 1 : 0;
    uint8_t pwm = _pwm->getPWM();

    unsigned long ts = millis();

    String json = "{";
    json += "\"ID\":\"" + _id + "\",";
    json += "\"timeStamp\":" + String(ts) + ",";
    json += "\"gas\":" + String(gas, 2) + ",";
    json += "\"temp_bmp\":" + String(temp_bmp, 2) + ",";
    json += "\"presion\":" + String(presion, 2) + ",";
    json += "\"temp_aht\":" + String(temp_aht, 2) + ",";
    json += "\"humedad\":" + String(humedad, 2) + ",";
    json += "\"luz\":" + String(luz) + ",";
    json += "\"rele1\":" + String(rele1) + ",";
    json += "\"rele2\":" + String(rele2) + ",";
    json += "\"pwm\":" + String(pwm);
    json += "}";

    return json;
}
