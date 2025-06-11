# 📂 Prototipo Principal: **OmniSens**

La carpeta `OmniSens` contiene el código fuente y la documentación del prototipo funcional principal del proyecto. Integra todos los componentes clave:

- **Dispositivos de borde** (sensores y concentrador)
- **Backend** (API)
- **Base de datos**
- **Capa de visualización**

---

## 🏗️ Arquitectura del Prototipo

El flujo de trabajo de este prototipo es el siguiente:

1. **Nodo Sensor (Edge Controller)**
    - Microcontrolador **ESP32** con múltiples sensores.
    - Captura datos ambientales.

2. **Comunicación LoRa**
    - El Nodo Sensor envía los datos al Concentrador usando LoRa.

3. **Concentrador LoRa (Hub)**
    - Otro **ESP32** que recibe la información de los nodos.
    - Procesa y publica los datos en un tópico específico de un **broker MQTT**.

4. **Backend (API)**
    - Servicio **Node.js** suscrito al tópico MQTT.
    - Procesa y almacena los datos en **MySQL**.

5. **Base de Datos**
    - Almacena información de dispositivos y lecturas de sensores.

6. **Visualización**
    - Un dashboard (Grafana) conectado a la base de datos muestra los datos en tiempo real.

---

## 📦 Componentes

A continuación, se detalla el contenido de cada subcarpeta principal dentro de `C-Prototipos/OmniSens`:

---

### **API (nueva)/**
Contiene el código fuente del servidor backend.

- **Tecnología:** Node.js
- **Framework:** Express.js
- **Funcionalidad Principal:**
  - API REST para gestión de dispositivos (`/api/devices`)
  - Conexión a broker MQTT para recibir telemetría
  - Uso de `mysql2` para interactuar con MySQL
- **Punto de Entrada:** `server.js`
- **Estructura del Código:**
  - `src/controllers`: Lógica de solicitudes HTTP
  - `src/routes`: Definición de rutas de la API
  - `src/service`: Lógica de negocio (servicios de base de datos y MQTT)

---

### **Base de datos/**
Define la estructura de la base de datos del proyecto.

- **Archivo Principal:** `OmniSens.sql`
- **Descripción:** Script SQL que crea la base de datos y define las tablas principales:

  | Tabla        | Descripción                                 | Campos Principales                                      |
  |--------------|---------------------------------------------|---------------------------------------------------------|
  | `devices`    | Registro de dispositivos IoT                | `id`, `location`, `createdAt`, `updatedAt`              |
  | `sensordata` | Lecturas de sensores                        | `id`, `device_id`, `sensor_type`, `value`, `timestamp`  |

---

### **docs/Edge/**
Contiene el firmware para los dispositivos embebidos (PlatformIO).

- **concentrador_LoRa/**
  - **Hardware:** Heltec WiFi LoRa 32
  - **Responsabilidades:**
     - Inicializa LoRa para recibir datos
     - Conexión WiFi y MQTT (PubSubClient)
     - Deserializa paquetes LoRa (JSON) y publica en MQTT

- **controlador_edge/**
  - **Hardware:** ESP32 (NodeMCU-32S)
  - **Sensores Integrados:**
     - **AHT25:** Humedad y Temperatura
     - **BMP280:** Presión atmosférica
     - **MQ135:** Calidad del Aire (CO₂)
  - **Responsabilidades:**
     - Lectura de sensores
     - Empaquetado de datos en JSON
     - Envío de datos por LoRa al concentrador

- **controlador_edge_2/**
  - **Hardware:** ESP8266 (NodeMCU-12E)
  - **Sensores Integrados:**
     - **BH1750:** Luminosidad
     - **DHT11:** Humedad y Temperatura
  - **Responsabilidades:**
     - Lectura de sensores
     - Empaquetado de datos en JSON
     - Envío de datos por LoRa al concentrador

- **concentrador_LoRa_2/**
  - **Hardware:** Heltec WiFi LoRa 32
  - **Responsabilidades:**
     - Inicializa LoRa para recibir datos
     - Conexión WiFi y MQTT (PubSubClient)
     - Deserializa paquetes LoRa (JSON) y publica en MQTT

## 📂 Estructura del directorio

```
Omnisens/docs/Edge/
├── sensor-edge/         # Código para nodo sensor (ESP32)
├── gateway-edge/        # Código para gateway LoRa-MQTT
├── actuator-edge/       # Código para actuadores (salidas GPIO)
└── README.md            # Documentación actual
```

### 🧰 Software

* [PlatformIO](https://platformio.org/)
* Repositorio clonado:

  ```bash
  git clone https://github.com/Programador-FullStack-IoT/Modulo-Programador-FullStack-IoT-TST-2024.git
  ```
* Docker con servicios activos (ver Portainer):
  🕻 [http://telecomunicaciones.ddns.net:8080/portainer/#!/3/docker/containers](http://telecomunicaciones.ddns.net:8080/portainer/#!/3/docker/containers)

---

### 🔧 Hardware

* Módulos ESP32, LoRa SX1278
* Sensores (temperatura, gas, humedad)
* Actuadores (relés, LED, motores)
* Cableado y protoboard

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
* Ruta: `/nodered_plata/ui/`: [Dashboard UI](http://telecomunicaciones.ddns.net:8080/nodered_plata/ui/#!/0)
* Incluye botones para control manual, recepción de datos, lógica de automatización.

### Grafana

* Dashboards visuales conectados a base de datos.
* Panel con temperatura, gas, humedad y estado de actuadores.
* Ruta directa:
  [Dashboard público](http://telecomunicaciones.ddns.net:8080/grafana/public-dashboards/51aba874b6f44eff8c361c8ffdba9956)

---

## ✅ Checklist de puesta en marcha

1. Clonar repositorio.
2. Instalar PlatformIO y dependencias.
3. Flashear nodos sensor y actuador.
4. Levantar contenedores Docker (ya disponibles en Portainer).
5. Confirmar flujo MQTT → Node‑RED → Grafana.
6. Validar comandos de actuación vía UI.

   ## 📢 Contacto

**Grupo Plata - Tecnicatura en Telecomunicaciones**
Repositorio: [OmniSens - FullStack IoT](https://github.com/Programador-FullStack-IoT/Modulo-Programador-FullStack-IoT-TST-2024)
Soporte: vía canal de clase o issues del repositorio.

> ℹ️ **Explora cada carpeta para más detalles sobre la implementación y configuración de cada componente.**
