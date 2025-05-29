#ifndef LCDPARALLELDISPLAY_H
#define LCDPARALLELDISPLAY_H

#include <LiquidCrystal.h>

class LCDParallelDisplay {
private:
    LiquidCrystal lcd; // Objeto de la librería LiquidCrystal
    int rs, rw, d4, d5, d6, d7; // Pines de control y datos del LCD

public:
    // Constructor: Inicializa el objeto con los pines
    LCDParallelDisplay(int rs, int rw, int d4, int d5, int d6, int d7);

    // Inicializa la pantalla LCD
    void begin();

    // Muestra el menú en la primera línea del LCD
    void displayMenu(int menu);

    // Actualiza la segunda línea del LCD con los datos del menú seleccionado
    void updateMenu(int menu, float mq135, float temp, float humidity, float bmpTemp, float pressure, float light, float uv);
};

#endif
