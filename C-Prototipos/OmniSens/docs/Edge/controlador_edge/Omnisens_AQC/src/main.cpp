#include <Arduino.h>
#include <Wire.h>
#include <Ticker.h>
#include "MQ135Sensor.h"
#include "AHT25Sensor.h"
#include "BMP280Sensor.h"
//#include "Baliza_con.h"
#include "LDRSensor.h"
#include "SalidaPWM.h"
#include "SalidasRele.h"
#include "LoRaComm.h"
#include "SensorData.h"

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
//Baliza_con baliza(WS2812_PIN);
LDRSensor ldr(LDR_PIN);
SalidaPWM vel_motor(PWM_PIN);
SalidasRele salidas(RELE_1, RELE_2);



// LoRa
//const char* NODE_ID = "AQC_001";
//const char* AUTH_TOKEN = "clave123";
LoRaComm lora;

//SensorData 
SensorData sensorData("AQC_001", &mq135, &bmp280, &aht25, &ldr, &salidas, &vel_motor);


Ticker envioTicker; // Ticker para enviar datos

volatile bool enviarFlag = false;





// --- SETUP ---
void setup() {
    Serial.begin(115200);
    while (!Serial);

    Wire.begin();
    if (!mq135.begin()) Serial.println("[Error] MQ135 no inicializado");
    if (!aht25.begin()) Serial.println("[Error] AHT25 no inicializado");
    if (!bmp280.begin()) Serial.println("[Error] BMP280 no inicializado");
    ldr.begin();
    vel_motor.begin();
    vel_motor.comandoPWM(0); // Inicializa el PWM en 0%
    salidas.begin();
    //baliza.begin();

    if (lora.begin()) {
        Serial.println("[LoRa] Inicializado correctamente");
    } else {
        Serial.println("[LoRa] Error al inicializar");
    }  

    
    
    // Timer cada 5s
    envioTicker.attach(5, []() {enviarFlag = true;});

    Serial.println("[Sistema] Inicialización completa");
}

// --- LOOP ---
void loop() {
  if (enviarFlag) {
    enviarFlag = false;

    String json = sensorData.toJSON();
    Serial.println("[JSON]: " + json);

    lora.sendMessage(json);
    Serial.println("[LoRa]: Mensaje enviado");
  }

  // Otros procesos no bloqueantes si los hay...
}


