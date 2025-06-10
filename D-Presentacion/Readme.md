# OmniSens: Plataforma de Monitoreo Ambiental IoT

![Estado del Proyecto](https://img.shields.io/badge/estado-activo-brightgreen)


OmniSens es una plataforma integral de monitoreo ambiental basada en IoT, diseñada para recolectar, almacenar y visualizar datos de diversos sensores. La plataforma está compuesta por dispositivos de borde (nodos sensores), un concentrador LoRa, una API de backend, una base de datos y un panel de visualización en Grafana.

Este proyecto ha sido desarrollado como parte del Módulo Full Stack IoT de la Tecnicatura Superior en Telecomunicaciones del ISPC.

---

## 📜 Tabla de Contenidos

1.  [📝 Resumen del Proyecto](#-resumen-del-proyecto)
2.  [🚀 Estado del Proyecto](#-estado-del-proyecto)
3.  [🛠️ Tecnologias Utilizadas](#-tecnologias-utilizadas)
4.  [📋 Requisitos Previos](#-requisitos-previos)
5.  [⚙️ Instalación](#-instalación)
6.  [🔧 Configuración](#-configuración)
7.  [▶️ Uso](#-uso)
8.  [🔌 Referencia de la API](#-referencia-de-la-api)
9. [🤝 Cómo Contribuir](#-cómo-contribuir)

---

## 📝 Resumen del Proyecto

El proyecto OmniSens tiene como objetivo principal monitorear la calidad del aire y otras variables ambientales utilizando una red de sensores IoT. Los datos son transmitidos a través de LoRa a un concentrador que los envía a un backend mediante MQTT. Finalmente, la información se almacena en una base de datos MySQL y se visualiza en tiempo real en un dashboard de Grafana.

### Características Principales

* **Comunicación Inalámbrica de Largo Alcance:** Utiliza LoRa para la comunicación entre los nodos sensores y el concentrador.
* **Protocolo Ligero:** Emplea MQTT para una comunicación eficiente entre el hardware y el backend.
* **API Robusta:** Un backend desarrollado en Node.js con Express para gestionar los datos y la lógica de negocio.
* **Almacenamiento Persistente:** Base de datos MySQL para el almacenamiento de los datos de los sensores.
* **Visualización en Tiempo Real:** Dashboard en Grafana para el monitoreo interactivo de las variables.
* **Arquitectura Escalable:** Diseño modular que permite añadir nuevos sensores y funcionalidades fácilmente.

---

## 🚀 Estado del Proyecto

**Activo**. El proyecto está en desarrollo activo y se están añadiendo nuevas funcionalidades. No hay una hoja de ruta pública definida, pero se planea continuar mejorando la plataforma.

---

## 🛠️ Tecnologías Utilizadas

### Backend

* **Node.js:** Entorno de ejecución para JavaScript del lado del servidor.
* **Express.js:** Framework web para construir la API REST.
* **mysql2:** Cliente de MySQL para Node.js, utilizado para la interacción con la base de datos.
* **mqtt.js:** Cliente MQTT para la comunicación con el concentrador.
* **dotenv:** Para la gestión de variables de entorno.
* **Helmet:** Para asegurar la aplicación Express estableciendo varias cabeceras HTTP.

### Frontend y Visualización

* **Grafana:** Plataforma de código abierto para la visualización y análisis de datos.

### Dispositivos Embebidos (Edge)

* **C++:** Lenguaje principal para la programación de los microcontroladores.
* **PlatformIO:** Ecosistema de desarrollo para software embebido.
* **ESP32 y ESP8266:** Microcontroladores utilizados para los nodos sensores y el concentrador.
* **LoRa (RadioLib):** Librería para la comunicación LoRa.
* **Sensores:**
    * **AHT25/AHT10:** Temperatura y humedad.
    * **BMP280:** Presión atmosférica.
    * **MQ135:** Calidad del aire (CO2, etc.).
    * **BH1750/GY-302:** Intensidad lumínica.

### Base de Datos

* **MySQL:** Sistema de gestión de bases de datos relacional.

---

## 📋 Requisitos Previos

Asegúrate de tener instalado el siguiente software antes de empezar:

* **Node.js:** v18 o superior.
* **NPM:** Gestor de paquetes de Node.js (normalmente se instala con Node.js).
* **PlatformIO Core:** Para compilar y subir el firmware a los dispositivos.
* **Docker (Recomendado):** Para levantar fácilmente el stack de backend (MySQL, Grafana, etc.).
* **Un cliente de MySQL:** Como MySQL Workbench o DBeaver (opcional).

---

## ⚙️ Instalación

1.  **Clonar el repositorio:**
    ```bash
    git clone [https://github.com/Programador-FullStack-IoT/Modulo-Programador-FullStack-IoT-TST-2024.git](https://github.com/Programador-FullStack-IoT/Modulo-Programador-FullStack-IoT-TST-2024.git)
    cd Modulo-Programador-FullStack-IoT-TST-2024
    ```

2.  **Instalar dependencias del Backend:**
    ```bash
    cd C-Prototipos/OmniSens/API\ (nueva)/
    npm install
    ```

3.  **Instalar librerías de PlatformIO:**
    PlatformIO instalará automáticamente las dependencias de las librerías al compilar los proyectos embebidos.

---

## 🔧 Configuración

### Backend

1.  Navega al directorio de la API: `C-Prototipos/OmniSens/API (nueva)/`.
2.  Crea un archivo `.env` a partir del archivo `.env.example` (si existiera). Basado en el código fuente (`src/config/index.js`), las variables de entorno necesarias son:
    ```env
    # Credenciales de la Base de Datos
    DB_HOST=localhost
    DB_USER=root
    DB_PASSWORD=tu_contraseña_secreta
    DB_DATABASE=OmniSens
    DB_PORT=3306

    # Configuración del Broker MQTT
    MQTT_BROKER=mqtt://localhost:1883
    MQTT_USERNAME=usuario_mqtt
    MQTT_PASSWORD=contraseña_mqtt

    # Puerto de la API
    PORT=3000
    ```

3.  **Base de Datos:**
    * Asegúrate de tener un servidor MySQL en ejecución.
    * Crea una base de datos llamada `OmniSens`.
    * Importa el esquema de la base de datos desde el archivo `C-Prototipos/OmniSens/Base de datos/OmniSens.sql`.

### Dispositivos Embebidos

Cada prototipo en `C-Prototipos/` tiene su propio archivo `platformio.ini` que define la configuración específica del hardware (pines, librerías, etc.). Asegúrate de revisar estos archivos antes de compilar y subir el firmware.

---

## ▶️ Uso

### Backend

Para iniciar el servidor de la API, ejecuta el siguiente comando desde el directorio `C-Prototipos/OmniSens/API (nueva)/`:

npm start

### Dispositivos Embebidos

1.  Abre el proyecto deseado (por ejemplo, `C-Prototipos/OmniSens/docs/Edge/controlador_edge/Omnisens_AQC/`) en VSCode con la extensión de PlatformIO.
2.  Conecta el dispositivo (ESP32) a tu computadora.
3.  Utiliza los comandos de PlatformIO para compilar y subir el firmware.

---

## 🔌 Referencia de la API

La API REST gestiona los dispositivos y los datos de los sensores.

### Endpoints de Dispositivos (`/api/devices`)

* **`GET /`**: Obtiene una lista de todos los dispositivos.
* **`POST /`**: Crea un nuevo dispositivo.
    * **Body:** `{ "location": "string" }`
* **`GET /:id`**: Obtiene un dispositivo por su ID.
* **`PUT /:id`**: Actualiza un dispositivo por su ID.
    * **Body:** `{ "location": "string" }`
* **`DELETE /:id`**: Elimina un dispositivo por su ID.

### Endpoints de Datos de Sensores (`/api/sensor-data`)

* **`POST /`**: Guarda nuevos datos de sensores. Este endpoint es utilizado por el broker MQTT para registrar la telemetría.
    * **Body:** `{ "device_id": "int", "sensor_type": "string", "value": "float" }`

---

## 🤝 Cómo Contribuir

Las contribuciones son bienvenidas. Si deseas contribuir, por favor sigue estos pasos:

1.  Haz un "fork" del repositorio.
2.  Crea una nueva rama para tu funcionalidad (`git checkout -b feature/nueva-funcionalidad`).
3.  Realiza tus cambios y haz "commit" de ellos (`git commit -am 'Añade nueva funcionalidad'`).
4.  Empuja tus cambios a tu rama (`git push origin feature/nueva-funcionalidad`).
5.  Crea un nuevo "Pull Request".

Asegúrate de que tu código siga las guías de estilo del proyecto y de que todas las pruebas pasen.

---
