#ifndef BALIZA_CON_H
#define BALIZA_CON_H

#include <Adafruit_NeoPixel.h>
#include <Ticker.h>

/**
 * @brief Estados posibles para cada color de la baliza.
 */
enum BalizaEstado {
    BALIZA_OFF = 0,
    BALIZA_ON,
    BALIZA_BLINK_SLOW,
    BALIZA_BLINK_FAST
};

/**
 * @brief Estructura para definir el patrón de un evento.
 */
struct BalizaEvento {
    BalizaEstado verde;
    BalizaEstado amarillo;
    BalizaEstado rojo;
};

class Baliza_con {
public:
    /**
     * @brief Constructor.
     * @param pin Pin de datos de la tira WS2812.
     */
    Baliza_con(int pin);

    /**
     * @brief Inicializa la tira de LEDs y el ticker de actualización.
     */
    void begin();

    /**
     * @brief Nuevo método para manejar actualizaciones
     */
    void update();
    /**
     * @brief Muestra el evento correspondiente en la baliza.
     * @param codigoEvento Código de evento según la tabla.
     * @param rele1 Estado del relé 1 (para advertencias).(se elimina en la versión final)
     * @param rele2 Estado del relé 2 (para advertencias).(se elimina en la versión final)
     */
    void mostrarEvento(uint8_t codigoEvento/*, bool rele1 = false, bool rele2 = false*/);

private:
    Adafruit_NeoPixel strip;
    Ticker ticker;
    bool tickerInicializado = false; // Indica si el ticker ha sido inicializado
    BalizaEvento eventoActual;
    bool blinkState; // Estado de parpadeo
    uint8_t codigoEventoActual;
    bool rele1Activo, rele2Activo;
    
    
    void actualizar();
    void setColor(uint8_t led, uint8_t r, uint8_t g, uint8_t b);
    void aplicarPatron();
    static const BalizaEvento tablaEventos[];
};

#endif