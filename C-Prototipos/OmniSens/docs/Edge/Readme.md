# 🚀 OmniSens - Módulo Edge (Grupo Plata)

Este módulo forma parte del proyecto **OmniSens** y contiene el desarrollo de los nodos *edge*: sensores, gateway y actuadores conectados a una red IoT. La implementación corresponde al grupo **Plata** de la Tecnicatura en Telecomunicaciones.

---

## 📂 Estructura del directorio

```
Omnisens/docs/Edge/
├── sensor-edge/         # Código para nodo sensor (ESP32)
├── gateway-edge/        # Código para gateway LoRa-MQTT
├── actuator-edge/       # Código para actuadores (salidas GPIO)
└── README.md            # Documentación actual
```

---

## ⚙️ Requisitos previos

### 🔧 Hardware

* Módulos ESP32, LoRa SX1278
* Sensores (temperatura, gas, humedad)
* Actuadores (relés, LED, motores)
* Cableado y protoboard

### 🧰 Software

* [PlatformIO](https://platformio.org/)
* Repositorio clonado:

  ```bash
  git clone https://github.com/Programador-FullStack-IoT/Modulo-Programador-FullStack-IoT-TST-2024.git
  ```
* Docker con servicios activos (ver Portainer):
  🕻 [http://telecomunicaciones.ddns.net:8080/portainer/#!/3/docker/containers](http://telecomunicaciones.ddns.net:8080/portainer/#!/3/docker/containers)

---

## 🚀 Instalación y configuración

### 1. Sensor Edge

Ubicado en `sensor-edge/`:

* Ajustar `platformio.ini` según placa.
* Configurar parámetros en `config.h`:

  ```cpp
  #define NODE_ID "sensor_01"
  #define LORA_FREQUENCY 915E6
  #define MQTT_TOPIC "plata/sensores"
  ```
* Compilar y flashear:

  ```bash
  pio run -t upload
  ```

### 2. Gateway Edge

Ubicado en `gateway-edge/`:

* Recibe datos LoRa y los reenvía vía MQTT.
* Configurar `mqtt_config.json`:

  ```json
  {
    "host": "broker",
    "port": 1883,
    "topic_out": "plata/sensores",
    "topic_in": "plata/actuadores"
  }
  ```

### 3. Actuator Edge

Ubicado en `actuator-edge/`:

* Suscripción al tópico `plata/actuadores`.
* Acciona salidas GPIO (ej: encender ventilador):

  ```cpp
  if (command == "ON") digitalWrite(RELAY_PIN, HIGH);
  ```

---

## 🧪 Pruebas y verificación

### 🌐 Servicios desplegados

* **Portainer**: [Contenedores activos](http://telecomunicaciones.ddns.net:8080/portainer/#!/3/docker/containers)
* **Node-RED (grupo Plata)**: [Dashboard UI](http://telecomunicaciones.ddns.net:8080/nodered_plata/ui/#!/0)
* **Grafana (Grupo Plata)**: [Panel Público](http://telecomunicaciones.ddns.net:8080/grafana/public-dashboards/51aba874b6f44eff8c361c8ffdba9956)

### 🔀 Flujo de datos

1. El sensor transmite vía LoRa al gateway.
2. El gateway reenvía datos al broker MQTT.
3. Node-RED suscribe datos y los visualiza.
4. Grafana extrae métricas de la base de datos.
5. Node-RED también emite comandos a los actuadores.

---

## 📊 Integración con Node-RED y Grafana

### Node-RED

* Flujo: MQTT → procesamiento → UI → actuador.
* Ruta: `/nodered_plata/ui/`
* Incluye botones para control manual, recepción de datos, lógica de automatización.

### Grafana

* Dashboards visuales conectados a base de datos.
* Panel con temperatura, gas, humedad y estado de actuadores.
* Ruta directa:
  [Dashboard público](http://telecomunicaciones.ddns.net:8080/grafana/public-dashboards/51aba874b6f44eff8c361c8ffdba9956)

---

## 🚯 Errores comunes

| Problema                    | Solución                                         |
| --------------------------- | ------------------------------------------------ |
| No compila                  | Verificar placa, puerto COM, librerías          |
| No recibe datos en Node-RED | Revisar frecuencia LoRa, conexión Gateway-MQTT   |
| Actuador no responde        | Verificar pines, lógica en Node-RED, MQTT activo |

---

## ✅ Checklist de puesta en marcha

1. Clonar repositorio.
2. Instalar PlatformIO y dependencias.
3. Flashear nodos sensor y actuador.
4. Levantar contenedores Docker (ya disponibles en Portainer).
5. Confirmar flujo MQTT → Node‑RED → Grafana.
6. Validar comandos de actuación vía UI.

---

## 🚣️ Roadmap y mejoras futuras

* Añadir autenticación MQTT y cifrado.
* Incluir alertas en Node-RED (vía correo o Telegram).
* Implementar dashboards privados por grupo.
* Documentación de integración global en `docs/integration.md`.

---

## 📢 Contacto

**Grupo Plata - Tecnicatura en Telecomunicaciones**
Repositorio: [OmniSens - FullStack IoT](https://github.com/Programador-FullStack-IoT/Modulo-Programador-FullStack-IoT-TST-2024)
Soporte: vía canal de clase o issues del repositorio.
