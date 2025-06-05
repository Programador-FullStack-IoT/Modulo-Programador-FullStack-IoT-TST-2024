#include "SensorData.h"

SensorData::SensorData(String id, MQ135Sensor* mq, BMP280Sensor* bmp, AHT25Sensor* aht,LDRSensor* ldr, SalidasRele* rele, SalidaPWM* pwm)
    : _id(id), _mq(mq), _bmp(bmp), _aht(aht), _ldr(ldr), _rele(rele), _pwm(pwm) {}   ///*  */

String SensorData::toJSON() {float gas = _mq ? _mq->readFilteredData() : -1.0;
    int mq135_ok = (!isnan(gas) && gas > 0.0) ? 1 : 0;
    if (!mq135_ok) gas = -1.0;

    float temp_bmp = -1.0, presion = -1.0;
    int bmp280_ok = _bmp && _bmp->readData(temp_bmp, presion) ? 1 : 0;
    if (!bmp280_ok) {
        temp_bmp = -1.0;
        presion = -1.0;
    }

    float temp_aht = -1.0, humedad = -1.0;
    int aht25_ok = _aht && _aht->readData(temp_aht, humedad) ? 1 : 0;
    if (!aht25_ok) {
        temp_aht = -1.0;
        humedad = -1.0;
    }

    int luz = _ldr ? _ldr->read() : -1;
    int rele1 = _rele ? (_rele->getRele1() ? 1 : 0) : 0;
    int rele2 = _rele ? (_rele->getRele2() ? 1 : 0) : 0;
    uint8_t pwm = _pwm ? _pwm->getPWM() : 0;

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
    json += "\"pwm\":" + String(pwm) + ",";
    json += "\"sensor_status\":{";
    json += "\"mq135\":" + String(mq135_ok) + ",";
    json += "\"bmp280\":" + String(bmp280_ok) + ",";
    json += "\"aht25\":" + String(aht25_ok);
    json += "}}";

    return json;
}
