#include "LCDParallelDisplay.h"

LCDParallelDisplay::LCDParallelDisplay(int rs, int rw, int d4, int d5, int d6, int d7)
    : lcd(rs, rw, d4, d5, d6, d7), rs(rs), rw(rw), d4(d4), d5(d5), d6(d6), d7(d7) {}

void LCDParallelDisplay::begin() {
    lcd.begin(16, 2); // Inicializa el LCD de 2x16 caracteres
    lcd.clear();
}

void LCDParallelDisplay::displayMenu(int menu) {
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

void LCDParallelDisplay::updateMenu(int menu, float mq135, float temp, float humidity, float bmpTemp, float pressure, float light, float uv) {
    lcd.setCursor(0, 1); // Mover a la segunda línea del LCD
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
