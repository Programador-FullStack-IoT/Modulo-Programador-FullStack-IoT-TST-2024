//nodo "control_edge"

#include <Arduino.h>
#include <SPI.h>
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
//Baliza_con baliza(WS2812_PIN);
LDRSensor ldr(LDR_PIN);
SalidaPWM vel_motor(PWM_PIN);
SalidasRele salidas(RELE_1, RELE_2);



// LoRa
const String NODE_ID = "AQC_001";
//const char* AUTH_TOKEN = "clave123";
LoRaComm lora(433E6); 

//SensorData 
SensorData sensorData(NODE_ID, &mq135, &bmp280, &aht25, &ldr, &salidas, &vel_motor);
DatoJson datoJson;

Ticker envioTicker; // Ticker para enviar datos

volatile bool enviarFlag = false;





// --- SETUP ---
void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("[Sistema] Inicializando...");
    Serial.println("[Sistema] Configurando pines LoRa ...");

    // Inicialización manual de SPI (por seguridad)
    SPI.begin(18, 19, 23, LORA_CS);

    // Seteo explícito de pines (aunque LoRaComm ya los usa por defecto)
    lora.setPins(LORA_CS, LORA_RST, LORA_INT);
    Serial.println("[Sistema] Pines LoRa configurados");
    Serial.println("[Sistema] Inicializando modulo comunicacion LoRa ...");

    if (lora.begin()) {
        Serial.println("[Sistema]LoRa inicializado correctamente");
    } else {
        Serial.println("[Sistema] Fallo al iniciar LoRa");
    }

    Serial.println("[Sistema] Inicializando comunicacion I2C ...");
    
    // Inicialización de Wire (I2C) si es necesario
    Wire.setPins(21, 22);  // Ajusta los pines SDA y SCL según tu hardware
    Wire.begin();
    Serial.println("[Sistema] I2C inicializado");

    Serial.println("[Sistema] Iniciando sensores AHT25 y BMP280");
    if (!aht25.begin()) {
        Serial.println("[Sistema] Error al iniciar el sensor AHT25");
    } else {
        Serial.println("[Sistema] Sensor AHT25 inicializado correctamente");
    }
    if (!bmp280.begin()) {
        Serial.println("[Sistema] Error al iniciar el sensor BMP280");
    } else {
        Serial.println("[Sistema] Sensor BMP280 inicializado correctamente");
    }

    Serial.println("[Sistema] Iniciando sensor MQ135");

    if (!mq135.begin()) {
        Serial.println("[Sistema] Error al iniciar el sensor MQ135");
    } else {
        Serial.println("[Sistema] Sensor MQ135 inicializado correctamente");
    }


    Serial.println("[Sistema] Iniciando sensor LDR");
    ldr.begin();

    Serial.println("[Sistema] Inicializando salida PWM");
    vel_motor.begin();
    vel_motor.comandoPWM(0); // Inicializa el PWM en 0% de forma explicita
    
    Serial.println("[Sistema] Inicializando salidas de relé");
    salidas.begin();
    //baliza.begin();
    delay(1000); // Espera para asegurar que todo esté listo
    // Timer cada 5s
    envioTicker.attach(5, []() {enviarFlag = true;});

    Serial.println("[Sistema] Inicialización completa");
}

// --- LOOP ---
void loop() {
  
  // --- Recibir comandos ---
  //String orden = lora.receiveMessage();
  //if (orden.length() > 0) {
  if (lora.hasNewMessage()) {
    String orden = lora.getLastMessage();
    Serial.println("[LoRa] Comando recibido (callback): " + orden);

    uint8_t pwm;
    bool r1, r2;
    uint8_t cod;

    if (datoJson.procesarOrdenes(orden, NODE_ID, pwm, r1, r2, cod)) {
      vel_motor.comandoPWM(pwm);
      salidas.setRele1(r1);
      salidas.setRele2(r2);
      Serial.printf("[Acción] PWM=%d Rele1=%d Rele2=%d Alarma=%d\n", pwm, r1, r2, cod);
    } else {
      Serial.println("[LoRa] Comando no válido o no dirigido a este nodo.");
    }
  }

  if (enviarFlag) {
    enviarFlag = false;

    String json = sensorData.toJSON();
    Serial.println("[JSON]: " + json);

    lora.sendMessage(json);

    LoRa.receive(); // Volver a modo escucha inmediatamente después de enviar

    Serial.println("[LoRa]: Mensaje enviado");
    
  }


}



