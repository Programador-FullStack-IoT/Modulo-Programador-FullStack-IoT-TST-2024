#include <Arduino.h>
#include <Wire.h>
#include <Ticker.h>
#include "MQ135Sensor.h"
#include "AHT25Sensor.h"
#include "BMP280Sensor.h"
#include "Baliza_con.h"
#include "LDRSensor.h"
#include "SalidaPWM.h"
#include "SalidasRele.h"
#include "EnlaceLora.h"
#include "DatoJson.h"

// Pines LoRa (ajusta según tu hardware)
#define LORA_CS   5
#define LORA_RST  14
#define LORA_INT  26

// Configuración de Pines sensores/actuadores
#define MQ135_PIN 39
#define PWM_PIN 32
#define RELE_1 33
#define RELE_2 25
#define WS2812_PIN 27
#define LDR_PIN 36

// Instancias globales
MQ135Sensor mq135(MQ135_PIN);
AHT25Sensor aht25;
BMP280Sensor bmp280;
Baliza_con baliza(WS2812_PIN);
LDRSensor ldr(LDR_PIN);
SalidaPWM vel_motor(PWM_PIN);
SalidasRele salidas(RELE_1, RELE_2);
DatoJson envio;
DatoJson recibo;

// LoRa
const char* NODE_ID = "AQC_001";
const char* AUTH_TOKEN = "clave123";
EnlaceLora com_lora(NODE_ID, AUTH_TOKEN, LORA_CS, LORA_RST, LORA_INT);

// Variables de sensores/actuadores
float mq135_val = 0, aht_temp = 0, aht_hum = 0, bmp_temp = 0, bmp_pres = 0, luz = 0, uv = 0;
uint8_t pwm_val = 0;
bool rele1_val = false, rele2_val = false, alarmaGas = false;
uint8_t codigoAlarma = 0;

// JSON buffer
String ultimoJson;
volatile bool jsonActualizado = false;

// Tickers
Ticker sensorTicker;
Ticker salidaTicker;
Ticker loraTicker;

// --- Funciones de actualización ---
void actualizarSensores() {
    // Lee sensores, maneja errores y actualiza variables globales
    if (!mq135.begin()) Serial.println("[Error] MQ135 no inicializado");
    mq135_val = mq135.readFilteredData();

    if (!aht25.begin()) Serial.println("[Error] AHT25 no inicializado");
    aht25.readData(aht_temp, aht_hum);

    if (!bmp280.begin()) Serial.println("[Error] BMP280 no inicializado");
    bmp280.readData(bmp_temp, bmp_pres);

    luz = ldr.readLux();
    // uv = ... // Si hay sensor UV

    // Aquí se puede calcular alarmaGas y codigoAlarma según lógica
    // alarmaGas = ...
    // codigoAlarma = ...
}

void actualizarSalidas() {
    vel_motor.comandoPWM(pwm_val);
    salidas.setRele1(rele1_val);
    salidas.setRele2(rele2_val);
    // Actualiza visualización
    baliza.updateLED((int)mq135_val); // Ejemplo: usa mq135 como AQI
}

void actualizarJson() {
    ultimoJson = envio.generarJson(
        NODE_ID, (uint32_t)time(nullptr), AUTH_TOKEN,
        mq135_val, aht_temp, aht_hum, bmp_temp, bmp_pres,
        luz, uv, pwm_val, rele1_val, rele2_val, alarmaGas, codigoAlarma
    );
    jsonActualizado = true;
}

void enviarPorLora() {
    if (jsonActualizado) {
        com_lora.sendJson(ultimoJson);
        jsonActualizado = false;
    }
}

// --- Callback para recepción LoRa ---
void onJsonLora(const String& json) {
    uint8_t pwm; bool r1, r2; uint8_t codAlarma;
    if (recibo.procesarOrdenes(json, NODE_ID, pwm, r1, r2, codAlarma)) {
        pwm_val = pwm;
        rele1_val = r1;
        rele2_val = r2;
        codigoAlarma = codAlarma;
        Serial.println("[LoRa] Orden recibida y aplicada");
    } else {
        Serial.println("[LoRa] JSON recibido no contiene ordenes válidas");
    }
}

// --- SETUP ---
void setup() {
    Serial.begin(115200);
    while (!Serial);

    Wire.begin();
    mq135.begin();
    aht25.begin();
    bmp280.begin();
    ldr.begin();
    vel_motor.begin();
    salidas.begin();
    baliza.begin();

    com_lora.begin(433.0);
    com_lora.setDebug(true);
    com_lora.onJsonReceived(onJsonLora);

    // Timers periódicos
    sensorTicker.attach_ms(100, actualizarSensores);
    salidaTicker.attach_ms(100, actualizarSalidas);
    loraTicker.attach(5, enviarPorLora);

    Serial.println("[Sistema] Inicialización completa");
}

// --- LOOP ---
void loop() {
    com_lora.loop();
    actualizarJson(); // Siempre actualiza el JSON con los últimos datos
}