#include <Arduino.h>
#include <SPI.h>
#include "LoRaComm.h"
#include <Wire.h>
#include <Ticker.h>
#include "AHT25Sensor.h"
#include "BMP280Sensor.h"


// Pines LoRa (ajustá según tu hardware real)
#define LORA_CS   5
#define LORA_RST  14
#define LORA_DIO0 26

LoRaComm lora(433E6);

bool enviarFlag = false;

Ticker envioTicker;  

AHT25Sensor aht25;  // Sensor de temperatura y humedad

BMP280Sensor bmp280;  // Sensor de presión y temperatura

void setup() {
    Serial.begin(115200);
    delay(1000);  // Estabiliza el monitor serial

    Serial.println("[TEST] Iniciando prueba mínima LoRa");

    // Inicialización manual de SPI (por seguridad)
    SPI.begin(18, 19, 23, LORA_CS);

    // Seteo explícito de pines (aunque LoRaComm ya los usa por defecto)
    lora.setPins(LORA_CS, LORA_RST, LORA_DIO0);

    if (lora.begin()) {
        Serial.println("[TEST] LoRa inicializado correctamente");
    } else {
        Serial.println("[TEST] Fallo al iniciar LoRa");
    }

    // Inicialización de Wire (I2C) si es necesario
    Wire.setPins(21, 22);  // Ajusta los pines SDA y SCL según tu hardware
    Wire.begin();
    
    Serial.println("[TEST] Wire (I2C) inicializado");
    byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println(" !");
      nDevices++;
    }
    else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

    Serial.println("[TEST] Iniciando sensores AHT25 y BMP280");
    if (!aht25.begin()) {
        Serial.println("[TEST] Error al iniciar el sensor AHT25");
    } else {
        Serial.println("[TEST] Sensor AHT25 inicializado correctamente");
    }
    if (!bmp280.begin()) {
        Serial.println("[TEST] Error al iniciar el sensor BMP280");
    } else {
        Serial.println("[TEST] Sensor BMP280 inicializado correctamente");
    }

    delay(5000);  // Espera para estabilizar la comunicación

    envioTicker.attach(5, []() {enviarFlag = true;});
    Serial.println("[TEST] Configuración completa, listo para enviar mensajes");
    
}

void loop() {
    static unsigned long lastSend = 0;
    float temperature, humidity;
    float bmp_temperature, bmp_pressure;
    //if (millis() - lastSend > 5000) {
    //    lastSend = millis();
    if (enviarFlag) {
        enviarFlag = false;  // Reinicia el flag

        String msg = "Prueba desde LoRaComm";
        lora.sendMessage(msg);
        Serial.println("[TEST] Mensaje enviado por LoRa: " + msg);

        // Leer datos de los sensores
        aht25.readData(temperature, humidity);
        bmp280.readData(bmp_temperature, bmp_pressure);
        Serial.println("[TEST] Datos del sensor AHT25 - Temp: " + String(temperature) + "C, Hum: " + String(humidity) + "%");
        Serial.println("[TEST] Datos del sensor BMP280 - Temp: " + String(bmp_temperature) + "C, Pres: " + String(bmp_pressure) + "hPa");

        // Agregar datos de sensores al mensaje
        //msg += String(" | Temp: ") + temperature + String("C | Hum: ") + humidity + String("% | BMP Temp: ") + bmp_temperature + String("C | BMP Pres: ") + bmp_pressure + String("hPa");
        //lora.sendMessage(msg);
        //Serial.println("[TEST] Mensaje enviado por LoRa: " + msg);
    }

    // Recepción (opcional)
    String incoming = lora.receiveMessage();
    if (incoming.length() > 0) {
        Serial.println("[TEST] Mensaje recibido: " + incoming);
    }
      

}
