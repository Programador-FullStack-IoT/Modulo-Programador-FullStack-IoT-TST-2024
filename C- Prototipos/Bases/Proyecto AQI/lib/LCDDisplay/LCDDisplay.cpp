#include "LCDDisplay.h"

LCDDisplay::LCDDisplay() : lcd(0x27, 16, 2) {}

void LCDDisplay::begin() {
    lcd.begin(16, 2, 0);
    lcd.backlight();
}

void LCDDisplay::displayMenu(int menu) {
    lcd.clear();
    switch (menu) {
        case 0:
            lcd.print("AQI: ");
            break;
        case 1:
            lcd.print("Temp/Hum: ");
            break;
        case 2:
            lcd.print("Luz/UV: ");
            break;
    }
}

void LCDDisplay::updateMenu(int menu, float mq135, float temp, float humidity, float bmpTemp, float pressure, float light, float uv) {
    lcd.setCursor(0, 1);
    switch (menu) {
        case 0:
            lcd.print("AQI: ");
            lcd.print(mq135);
            break;
        case 1:
            lcd.print("T: ");
            lcd.print(temp);
            lcd.print(" H: ");
            lcd.print(humidity);
            break;
        case 2:
            lcd.print("Luz: ");
            lcd.print(light);
            lcd.print(" UV: ");
            lcd.print(uv);
            break;
    }
}
