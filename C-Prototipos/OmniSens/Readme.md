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


> ℹ️ **Explora cada carpeta para más detalles sobre la implementación y configuración de cada componente.**