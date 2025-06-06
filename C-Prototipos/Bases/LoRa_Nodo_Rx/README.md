# Proyecto IoT - Comunicación por Radiofrecuencia LoRa con ESP32  

Este proyecto implementa una comunicación inalámbrica entre dos nodos ESP32 utilizando módulos LoRa SX1278. Un nodo transmisor mide temperatura y humedad con un sensor DHT11 y envía los datos al nodo receptor. Este último activa un relay (o LED) si la temperatura supera un umbral predefinido.

### 🔧 Componentes Utilizados  

- 2× ESP32 Dev Module

- 2× Módulo LoRa SX1278 (modelo Lora1278-SMT)

- 1× Sensor DHT11

- 1× Relay o LED (como actuador)

- Protoboard, cables dupont, fuente 5V

## ⚙️ Funcionalidad  

1. El nodo transmisor:

- Lee los datos del sensor DHT11 (temperatura y humedad).

- Envía los datos por LoRa en formato de texto plano, por ejemplo:  

Temp:31 Hum:60

2. El nodo receptor:

- Recibe el mensaje a través del módulo LoRa.

- Extrae el valor de temperatura desde el mensaje recibido.

- Enciende un relay o LED si la temperatura es mayor a 30 °C, de lo contrario, permanece apagado.

### 🔌 Conexiones Nodo Transmisor  

|Componente	|Pin ESP32|  
|-----------|---------|
|DHT11 Data	|GPIO 14|
|LoRa VCC	|3.3V|
|LoRa GND	|GND|
|LoRa SCK	|GPIO 18|
|LoRa MISO	|GPIO 19|
|LoRa MOSI	|GPIO 23|
|LoRa CS	|GPIO 5|
|LoRa RST	|GPIO 14|
|LoRa DIO0	|GPIO 26|

### 🔌 Conexiones Nodo Receptor  

|Componente	|Pin ESP32|  
|-----------|---------|
|Relay/LED	|GPIO 27|
|LoRa →	|Igual que en el transmisor|  


### 💾 Librerías necesarias (en platformio.ini)  

```ini
lib_deps =
  sandeepmistry/LoRa@^0.8.0
  adafruit/DHT sensor library@^1.4.3  
```  

### 📟 Código del Nodo Receptor (LoRa_NodoRx)
```cpp

#include <SPI.h>
#include <LoRa.h>

// Pines LoRa
#define LORA_SCK 18
#define LORA_MISO 19
#define LORA_MOSI 23
#define LORA_SS 5
#define LORA_RST 14
#define LORA_DIO0 26

#define RELAY_PIN 27  // Simula el relay con un LED

void setup() {
  Serial.begin(115200);
  while (!Serial);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Apagado al inicio

  // Configurar LoRa
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(433E6)) {
    Serial.println("Fallo al iniciar LoRa");
    while (1);
  }

  Serial.println("Nodo Receptor iniciado...");
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String incoming = "";
    while (LoRa.available()) {
      incoming += (char)LoRa.read();
    }

    Serial.print("Mensaje recibido: ");
    Serial.println(incoming);

    // Analizar datos. Supongamos que recibimos: "Temp:31 Hum:60"
    if (incoming.indexOf("Temp:") != -1) {
      int tempIndex = incoming.indexOf("Temp:") + 5;
      int spaceIndex = incoming.indexOf(' ', tempIndex);
      String tempStr = incoming.substring(tempIndex, spaceIndex);
      float temp = tempStr.toFloat();

      if (temp > 30) {
        digitalWrite(RELAY_PIN, HIGH); // Activa relay
        Serial.println(">>> Relay ON (Temp alta)");
      } else {
        digitalWrite(RELAY_PIN, LOW);
        Serial.println(">>> Relay OFF (Temp normal)");
      }
    }
  }
}
```