#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Ticker.h>

#include "MQ135Sensor.h"
#include "AHT25Sensor.h"
#include "BMP280Sensor.h"
#include "LDRSensor.h"
#include "SalidaPWM.h"
#include "SalidasRele.h"
#include "LoRaComm.h"
#include "SensorData.h"

// ======================= Pines ========================
#define LORA_CS   5
#define LORA_RST  27
#define LORA_DIO0 26

#define MQ135_PIN 39
#define LDR_PIN   36
#define PWM_PIN   32
#define RELE1_PIN 33
#define RELE2_PIN 25

// ======================= Objetos ========================
MQ135Sensor mq135(MQ135_PIN);
AHT25Sensor aht25;
BMP280Sensor bmp280;
LDRSensor ldr(LDR_PIN);
SalidaPWM pwm(PWM_PIN);
SalidasRele reles(RELE1_PIN, RELE2_PIN);

//======================== ID_controlador ========================
const String ID_CONTROLADOR = "AQC_001";

// LoRa
LoRaComm lora(433E6);

// SensorData
SensorData sensorData(ID_CONTROLADOR, &mq135, &bmp280, &aht25, &ldr, &reles, &pwm);

// Ticker
Ticker envioTicker;
volatile bool enviarFlag = false;

// ======================= Setup ========================
void setup() {
  Serial.begin(115200);
  delay(1000); // Esperar a estabilizar

  Serial.println("[Sistema] Iniciando...");

  // Inicializar SPI para LoRa
  //SPI.begin(18, 19, 23, LORA_CS);
  //lora.setPins(LORA_CS, LORA_RST, LORA_DIO0);
  //if (!lora.begin()) {
  //  Serial.println("[LoRa] Error al iniciar LoRa");
  //} else {
  //  Serial.println("[LoRa] Inicializado correctamente");
  //}

  // Inicializar I2C
  Wire.setPins(21, 22); // Configurar pines SDA y SCL
  Wire.begin(); // SDA: 21, SCL: 22 por defecto en ESP32

   //Inicializar sensores con validaciones
  if (!mq135.begin()) Serial.println("[ADVERTENCIA] MQ135 no inicializado");
  if (!aht25.begin()) Serial.println("[ADVERTENCIA] AHT25 no inicializado");
  if (!bmp280.begin()) Serial.println("[ADVERTENCIA] BMP280 no inicializado");

  //ldr.begin();
  //pwm.begin();
 // pwm.comandoPWM(0);  // PWM en 0% al inicio
  //reles.begin();

  envioTicker.attach(5, []() { enviarFlag = true; });

  Serial.println("[Sistema] Inicialización completa.");
}

// ======================= Loop ========================
void loop() {
  if (enviarFlag) {
    enviarFlag = false;

    String payload = sensorData.toJSON();
    Serial.println("[JSON]: " + payload);

    lora.sendMessage(payload);
    Serial.println("[LoRa] Mensaje enviado");
  }

  // Acá podrías agregar otras tareas si fueran necesarias.
}
