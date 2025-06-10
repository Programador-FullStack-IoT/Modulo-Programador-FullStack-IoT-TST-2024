# Nodo Actuador - OmniSens

Este microcontrolador (ESP32) se encarga de ejecutar comandos remotos enviados desde el sistema de control (dashboard) o directamente desde otro microcontrolador, mediante los siguientes canales:

- MQTT (tópicos específicos)
- LoRa (enlace directo punto a punto)

---

## ⚙️ Funciones del nodo

- Escucha comandos MQTT o LoRa con un formato JSON.
- Procesa el mensaje y extrae:
  - Nivel de PWM (motor, ventilador)
  - Estados de relé 1 y relé 2
  - Código de alarma (para luces o alertas)
- Aplica los valores recibidos a los actuadores conectados.
- Puede incluir retroalimentación (en desarrollo).

---

## 🧱 Actuadores controlados

| Actuador        | Descripción                       |
|------------------|----------------------------------|
| Relé 1           | Control de dispositivo eléctrico |
| Relé 2           | Control de segundo dispositivo   |
| PWM (Pin D4)     | Control de velocidad (ej. motor) |
| WS2812           | Baliza LED de estado o alarma    |

---

## 🔌 Pines típicos utilizados

| Componente     | Pin |
|----------------|-----|
| PWM (motor)    | D4  |
| Relé 1         | D0  |
| Relé 2         | D2  |
| WS2812         | D39 |

---

## 📦 Comandos esperados (formato JSON) 

```json
{
  "id": "AQC_001",
  "token": "clave123",
  "pwm": 150,
  "rele1": true,
  "rele2": false,
  "cod": 2
}
| Campo | Descripción                         |
| ----- | ----------------------------------- |
| id    | Identificador del nodo destinatario |
| token | Token de autenticación              |
| pwm   | Valor PWM (0 a 255)                 |
| rele1 | Estado del relé 1 (true/false)      |
| rele2 | Estado del relé 2 (true/false)      |
| cod   | Código de alarma o estado           |


## 🔁 Flujo funcional del nodo

[ MQTT o LoRa RX ]
         ↓
[ JSON recibido ]
         ↓
[ Decodificación ]
         ↓
[ Actuadores: PWM, Relés, WS2812 ]

## 💬 Comunicación bidireccional

- Este nodo puede:

- Escuchar y ejecutar órdenes remotas (actualmente implementado).

- En el futuro: enviar estado o alertas al sistema (a través de MQTT o LoRa).

## 📁 Ubicación del código

/edge/microcontrolador-actuador/
├── include/
│   ├── SalidaPWM.h
│   ├── SalidasRele.h
│   ├── Baliza_con.h
│   ├── EnlaceLora.h
│   └── DatoJson.h
├── src/
│   └── main.cpp
├── platformio.ini

## 🧠 Observaciones

- Es un nodo de ejecución de órdenes, no tiene sensores.

- Soporta ambos protocolos: LoRa y MQTT.

- La lógica de seguridad debe validar el id y token antes de aplicar los comandos.