#ifndef LCDDISPLAY_H
#define LCDDISPLAY_H

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class LCDDisplay {
private:
    LiquidCrystal_I2C lcd;

public:
    LCDDisplay();
    void begin();
    void displayMenu(int menu);
    void updateMenu(int menu, float mq135, float temp, float humidity, float bmpTemp, float pressure, float light, float uv);
};

#endif
