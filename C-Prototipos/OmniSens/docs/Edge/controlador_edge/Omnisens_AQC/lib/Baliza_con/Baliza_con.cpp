#include "Baliza_con.h"

// Tabla de eventos (puedes expandirla según tu tabla documentada)
const BalizaEvento Baliza_con::tablaEventos[] = {
    //  verde,         amarillo,           rojo
    {BALIZA_ON,        BALIZA_OFF,         BALIZA_OFF},        // 0: Normal
    {BALIZA_BLINK_SLOW,BALIZA_BLINK_SLOW,  BALIZA_OFF},        // 1: Inicializando
    {BALIZA_OFF,       BALIZA_BLINK_SLOW,  BALIZA_OFF},        // 2: Esperando conexión
    {BALIZA_OFF,       BALIZA_BLINK_FAST,  BALIZA_OFF},        // 3: Enviando datos
    {BALIZA_BLINK_FAST,BALIZA_BLINK_FAST,  BALIZA_OFF},        // 4: Recibiendo orden
    {BALIZA_OFF,       BALIZA_ON,          BALIZA_OFF},        // 5: Advertencia genérica
    {BALIZA_OFF,       BALIZA_OFF,         BALIZA_BLINK_FAST}, // 6: Alarma genérica
    // ...agrega más según tu tabla...
};

Baliza_con::Baliza_con(int pin)
    : strip(8, pin, NEO_GRB + NEO_KHZ800), blinkState(false), codigoEventoActual(0), rele1Activo(false), rele2Activo(false), ticker(nullptr, 0, 0, MILLIS) {

    }

void Baliza_con::begin() {
    strip.begin();
    strip.show();
    if (!tickerInicializado) {
        // Reconstruimos el objeto Ticker usando placement new
        new (&ticker) Ticker(
            [this]() { this->actualizar(); },  // Lambda como callback
            250,                               // Intervalo en ms
            0,                                 // Repeticiones infinitas
            MILLIS                             // Resolución en milisegundos
        );
        
        ticker.start();
        tickerInicializado = true;
    }
}

// Nuevo método de actualización
void Baliza_con::update() {
    if (tickerInicializado) {
        ticker.update();  // Actualizamos el ticker
    }
  }

void Baliza_con::mostrarEvento(uint8_t codigoEvento/*, bool rele1, bool rele2*/) {
    codigoEventoActual = codigoEvento;
    //rele1Activo = rele1;
    //rele2Activo = rele2;
    if (codigoEvento < sizeof(tablaEventos)/sizeof(tablaEventos[0])) {
        eventoActual = tablaEventos[codigoEvento];
    } else {
        eventoActual = {BALIZA_OFF, BALIZA_OFF, BALIZA_OFF};
    }
    aplicarPatron();
}

void Baliza_con::actualizar() {
    blinkState = !blinkState;
    aplicarPatron();
}

void Baliza_con::aplicarPatron() {
    // Verde: D0, D1
    for (uint8_t i = 0; i < 2; ++i) {
        switch (eventoActual.verde) {
            case BALIZA_ON:         setColor(i, 0, 255, 0); break;
            case BALIZA_BLINK_SLOW: setColor(i, 0, blinkState ? 255 : 0, 0); break;
            case BALIZA_BLINK_FAST: setColor(i, 0, blinkState ? 255 : 0, 0); break;
            default:                setColor(i, 0, 0, 0); break;
        }
    }
    // Amarillo: D2, D3
    for (uint8_t i = 2; i < 4; ++i) {
        switch (eventoActual.amarillo) {
            case BALIZA_ON:         setColor(i, 255, 255, 0); break;
            case BALIZA_BLINK_SLOW: setColor(i, blinkState ? 255 : 0, blinkState ? 255 : 0, 0); break;
            case BALIZA_BLINK_FAST: setColor(i, blinkState ? 255 : 0, blinkState ? 255 : 0, 0); break;
            default:                setColor(i, 0, 0, 0); break;
        }
    }
    // Rojo: D4, D5
    for (uint8_t i = 4; i < 6; ++i) {
        switch (eventoActual.rojo) {
            case BALIZA_ON:         setColor(i, 255, 0, 0); break;
            case BALIZA_BLINK_SLOW: setColor(i, blinkState ? 255 : 0, 0, 0); break;
            case BALIZA_BLINK_FAST: setColor(i, blinkState ? 255 : 0, 0, 0); break;
            default:                setColor(i, 0, 0, 0); break;
        }
    }
    // D6, D7: Copian D4, D5 solo si es alarma (rojo activo)
    bool esAlarma = (eventoActual.rojo == BALIZA_ON || eventoActual.rojo == BALIZA_BLINK_SLOW || eventoActual.rojo == BALIZA_BLINK_FAST);
    for (uint8_t i = 6; i < 8; ++i) {
        if (esAlarma) {
            uint8_t ref = i - 2; // D4 o D5
            uint32_t color = strip.getPixelColor(ref);
            strip.setPixelColor(i, color);
        } else {
            setColor(i, 0, 0, 0);
        }
    }
    // Opcional: Indicar relés en advertencias (ejemplo: azul para rele1, blanco para rele2)
    if (eventoActual.amarillo != BALIZA_OFF) {
        if (rele1Activo) setColor(0, 0, 0, 255); // D0 azul si rele1 activo
        if (rele2Activo) setColor(1, 255, 255, 255); // D1 blanco si rele2 activo
    }
    strip.show();
}

void Baliza_con::setColor(uint8_t led, uint8_t r, uint8_t g, uint8_t b) {
    strip.setPixelColor(led, strip.Color(r, g, b));
}
