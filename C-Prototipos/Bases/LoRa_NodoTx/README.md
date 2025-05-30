# Proyecto IoT - Comunicación por Radiofrecuencia LoRa con ESP32

Este proyecto implementa una comunicación inalámbrica entre dos nodos ESP32 utilizando módulos LoRa SX1278. Un nodo transmisor mide temperatura y humedad con un sensor DHT11 y envía los datos al nodo receptor, que acciona un relay (o LED) dependiendo del valor de temperatura.

## 🔧 Componentes Utilizados

- 2× ESP32 Dev Module  
- 2× Módulo LoRa SX1278 (modelo Lora1278-SMT)  
- 1× Sensor DHT11  
- 1× Relay o LED (como actuador)  
- Protoboard, cables dupont, fuente 5V

## ⚙️ Funcionalidad

- El **nodo transmisor**:
  - Lee los datos del sensor DHT11 (temperatura y humedad).
  - Envía los datos por LoRa en formato `Temp:xx.x,Hum:yy.y`.

- El **nodo receptor**:
  - Recibe los datos LoRa.
  - Analiza la temperatura recibida.
  - Enciende un relay o LED si la temperatura supera los 30 °C.

## 🔌 Conexiones Nodo Transmisor

| Componente | Pin ESP32 |
|------------|-----------|
| DHT11 Data | GPIO 14   |
| LoRa VCC   | 3.3V      |
| LoRa GND   | GND       |
| LoRa SCK   | GPIO 18   |
| LoRa MISO  | GPIO 19   |
| LoRa MOSI  | GPIO 23   |
| LoRa CS    | GPIO 5    |
| LoRa RST   | GPIO 14   |
| LoRa DIO0  | GPIO 26   |

## 🔌 Conexiones Nodo Receptor

| Componente | Pin ESP32 |
|------------|-----------|
| Relay/LED  | GPIO 27   |
| LoRa →     | Igual que en el transmisor |

## 💾 Librerías necesarias (en `platformio.ini`)

```ini
lib_deps =
  sandeepmistry/LoRa@^0.8.0
  adafruit/DHT sensor library@^1.4.3
