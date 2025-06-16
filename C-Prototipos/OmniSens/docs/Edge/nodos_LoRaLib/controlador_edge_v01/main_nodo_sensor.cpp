#include <Arduino.h>
#include <RadioLib.h>  // Usamos RadioLib en lugar de LoRaLib
#include <SPI.h>
#include <Wire.h>
#include <Ticker.h>
#include "MQ135Sensor.h"
#include "AHT25Sensor.h"
#include "BMP280Sensor.h"
#include "LDRSensor.h"
#include "SalidaPWM.h"
#include "SalidasRele.h"
#include "SensorData.h"
#include "DatoJson.h"

// --- Configuración Hardware ---
// Pines LoRa (ESP32)
#define LORA_SS    5
#define LORA_RST   14
#define LORA_DIO0  26
#define LORA_DIO1  34  // Opcional para interrupciones

// Pines Sensores/Actuadores
#define MQ135_PIN  39
#define PWM_PIN    32
#define RELE_1     33
#define RELE_2     25
#define WS2812_PIN 27
#define LDR_PIN    36

// --- Instancias Globales ---
// Sensores
MQ135Sensor mq135(MQ135_PIN);
AHT25Sensor aht25;
BMP280Sensor bmp280;
LDRSensor ldr(LDR_PIN);

// Actuadores
SalidaPWM vel_motor(PWM_PIN);
SalidasRele salidas(RELE_1, RELE_2);

// LoRa (RadioLib)
SX1278 radio = new Module(LORA_SS, LORA_DIO0, LORA_RST, LORA_DIO1);  // ¡Asegúrate de conectar DIO1!

// Datos y Comunicación
const String NODE_ID = "AQC_001";
SensorData sensorData(NODE_ID, &mq135, &bmp280, &aht25, &ldr, &salidas, &vel_motor);
DatoJson datoJson;

// Ticker para envío periódico
Ticker envioTicker;
volatile bool enviarFlag = false;

// --- Setup ---
void setup() {
  Serial.begin(115200);
  while (!Serial);  // Solo para depuración, quitar en producción

  // Inicializar I2C para sensores
  Wire.begin();
  aht25.begin();
  bmp280.begin();

  // Inicializar SPI para LoRa
  SPI.begin(18, 19, 23, LORA_SS);  // SCK=18, MISO=19, MOSI=23, SS=5 (ESP32)

  // Configurar Radio (Clase C - Recepción continua)
  int state = radio.begin(433.0, 125.0, 9, 5, 0xF3, 17);  // Freq=433MHz, BW=125kHz, SF=9, CR=4/5, SyncWord=0xF3, Power=17dBm
  if (state != RADIOLIB_ERR_NONE) {
    Serial.print("[LoRa] Error inicializando: ");
    Serial.println(state);
    while (true);  // Bloquea si hay error
  }
  radio.setRfSwitchPins(-1, -1);  // Opcional: si no usas control de antena
  Serial.println("[LoRa] Inicializado (Clase C)");

  // Configurar ticker para envío cada 5 segundos
  envioTicker.attach(5, []() { enviarFlag = true; });
}

// --- Loop Principal ---
void loop() {
  // 1) Enviar datos periódicos
  if (enviarFlag) {
    enviarFlag = false;
    String json = sensorData.toJSON();
    Serial.println("[LoRa] Enviando: " + json);

    // Verificar tamaño del JSON (máx. 255 bytes con RadioLib)
    if (json.length() > 240) {
      Serial.println("[ERROR] JSON demasiado grande");
      return;
    }

    // Enviar (sin ACK en Clase C, pero con retry)
    int state = radio.transmit(json);
    if (state != RADIOLIB_ERR_NONE) {
      Serial.print("[LoRa] Error en TX: ");
      Serial.println(state);
    }
  }

  // 2) Escucha continua (Clase C)
  String respuesta;
  int state = radio.receive(respuesta);  // Timeout=0 (no bloqueante)
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println("[LoRa] Orden recibida: " + respuesta);
    
    // Procesar orden (ejemplo)
    uint8_t pwm;
    bool r1, r2;
    uint8_t codAlarma;
    if (datoJson.procesarOrdenes(respuesta, NODE_ID, pwm, r1, r2, codAlarma)) {
      vel_motor.comandoPWM(pwm);      // Actualizar PWM
      salidas.setRele1(r1);       // Actualizar Relé 1
      salidas.setRele2(r2);       // Actualizar Relé 2
      // ... manejar codAlarma si es necesario
    }
  }
}