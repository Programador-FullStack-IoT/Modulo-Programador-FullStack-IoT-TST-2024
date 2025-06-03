#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <Arduino.h>
#include <MQ135Sensor.h>
#include <BMP280Sensor.h>
#include <AHT25Sensor.h>
#include <LDRSensor.h>
#include <SalidasRele.h>
#include <SalidaPWM.h>

class SensorData {
public:
    SensorData(String id, MQ135Sensor* mq, BMP280Sensor* bmp, AHT25Sensor* aht, LDRSensor* ldr,
               SalidasRele* rele, SalidaPWM* pwm);

    String toJSON();

private:
    String _id;
    MQ135Sensor* _mq;
    BMP280Sensor* _bmp;
    AHT25Sensor* _aht;
    LDRSensor* _ldr;
    SalidasRele* _rele;
    SalidaPWM* _pwm;
};

#endif
