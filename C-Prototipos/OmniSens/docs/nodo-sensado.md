# Nodo de Sensado - OmniSens

Este microcontrolador (ESP32) se encarga de:

- Leer variables ambientales: gases (MQ135), temperatura y humedad (AHT25), presión (BMP280), y luminosidad (LDR).
- Generar un JSON con todos los datos sensados.
- Enviar los datos vía LoRa a otro microcontrolador (nodo receptor).
- Recibir comandos (PWM, relés, alarmas) desde el nodo receptor también vía LoRa.

---

## 🔌 Pines utilizados

| Componente        | Pin |
|-------------------|-----|
| LoRa_CS           | 18  |
| LoRa_RST          | 14  |
| LoRa_INT          | 26  |
| MQ135 (gases)     | 34  |
| PWM (motor)       | 4   |
| Relé 1            | 0   |
| Relé 2            | 2   |
| WS2812 (baliza)   | 39  |
| LDR (luz)         | 36  |

---

## ⚙️ Funcionalidad del código (`main.cpp`)

- **Lectura de sensores:** se realiza con objetos de clases personalizadas:
  - `MQ135Sensor`
  - `AHT25Sensor`
  - `BMP280Sensor`
  - `LDRSensor`

- **Control de actuadores:**
  - PWM: salida analógica modulada.
  - Relé 1 y Relé 2: salidas digitales.
  - WS2812: LED inteligente para indicar estado ambiental (por ej. nivel de gas).

- **Timers no bloqueantes (`Ticker`):**
  - `actualizarSensores()` cada 100 ms.
  - `actualizarSalidas()` cada 100 ms.
  - `enviarPorLora()` cada 5 segundos.

- **Envío y recepción LoRa:**
  - Envío periódico del JSON con datos.
  - Recepción de comandos codificados en JSON (PWM, relés, alarma).

---

## 🧱 JSON generado

Se envía un JSON con esta información:
- Identificación del nodo (`NODE_ID`)
- Timestamp actual
- Valores de sensores (temperatura, humedad, presión, gases, luz, UV*)
- Estado de los actuadores
- Flags como `alarmaGas` y `codigoAlarma`

Ejemplo (formato):

```json
{
  "id": "AQC_001",
  "ts": 1710000000,
  "token": "clave123",
  "mq135": 305.7,
  "temp": 23.4,
  "hum": 45.1,
  "pres": 1012.3,
  "lux": 358,
  "uv": 0,
  "pwm": 120,
  "rele1": false,
  "rele2": true,
  "alarma": false,
  "cod": 0
}
