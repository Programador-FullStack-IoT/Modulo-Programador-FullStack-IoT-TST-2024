#include <Arduino.h>
#include <DHT.h>
#include <SPI.h>
#include <LoRa.h>

// --- Pines --- //
#define DHTPIN 4        // Pin de datos del DHT11
#define DHTTYPE DHT11   // Tipo de sensor

#define SS 5            // LoRa - NSS
#define RST 14          // LoRa - RESET
#define DIO0 26         // LoRa - DIO0

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // Inicializar DHT11
  dht.begin();
  Serial.println("Sensor DHT11 iniciado.");

  // Inicializar LoRa
  Serial.println("Iniciando LoRa...");
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(433E6)) {
    Serial.println("Error al iniciar LoRa. Verifique conexión.");
    while (1);
  }

  Serial.println("LoRa iniciado correctamente.");
}

void loop() {
  // Leer datos del sensor
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Fallo al leer el DHT11.");
    delay(2000);
    return;
  }

  // Mostrar por monitor serial
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.print(" °C | Humedad: ");
  Serial.print(h);
  Serial.println(" %");

  // Enviar por LoRa
  LoRa.beginPacket();
  LoRa.print("Temp:");
  LoRa.print(t);
  LoRa.print(" Hum:");
  LoRa.print(h);
  LoRa.endPacket();

  delay(5000); // Espera 5 segundos
}
