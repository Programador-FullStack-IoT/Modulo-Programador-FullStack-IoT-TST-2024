#ifndef SALIDASRELE_H
#define SALIDASRELE_H

#include <Arduino.h>

class SalidasRele {
public:
    SalidasRele(int pin1, int pin2);
    void begin();
    void setRele1(bool estado);
    void setRele2(bool estado);
    void toggleRele1();
    void toggleRele2();
    bool getRele1() const;
    bool getRele2() const;
private:
    int _pin1, _pin2;
    bool _estado1, _estado2;
};

#endif