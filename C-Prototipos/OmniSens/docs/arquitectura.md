# Arquitectura General del Proyecto OmniSens

Este documento describe la arquitectura lógica y física del sistema IoT desarrollado por el grupo AQI – PLATA, en el marco del Módulo FullStack IoT (Tecnicatura Superior en Telecomunicaciones).

---

## 🧱 Componentes del Sistema

| Componente                   | Descripción                                                         |
|-----------------------------|---------------------------------------------------------------------|
| Nodo de Sensado             | Dispositivo con sensores ambientales, genera y envía datos por LoRa |
| Nodo Receptor               | Recibe por LoRa y publica por MQTT                                  |
| Nodo Actuador               | Recibe comandos por MQTT o LoRa y activa relés, baliza, PWM         |
| Broker MQTT                 | Alojado en la nube para publicar/suscribirse a tópicos    |
| Node-RED                    | Motor de flujos para procesamiento, control y envío a base de datos |
| Base de Datos (MariaDB)     | Almacena datos históricos de sensores                               |
| Dashboard (UI/Grafana)      | Interfaz de visualización y control manual                          |

---

## 🔄 Flujo de Datos General

[ Nodo Sensado ]
↓ LoRa
[ Nodo Receptor ]
↓ MQTT
[ Broker Mosquitto ]
↓
[ Node-RED ]
↓ ↘
[ MariaDB ] [ MQTT → Nodo Actuador ]
↓
[ Actuadores ]


---

## 📚 Documentación Técnica por módulo

Para más detalle sobre cada nodo, consultar:

- 📗 [`nodo-sensado.md`](./nodo-sensado.md)
- 📘 [`nodo-receptor.md`](./nodo-receptor.md)
- 🔧 [`nodo-actuador.md`](./nodo-actuador.md) *(por crear)*
- 🌐 [`backend.md`](./backend.md) *(por crear)*
- 🖥️ [`frontend.md`](./frontend.md) *(por crear)*

---

## 🔌 Tecnologías utilizadas

- **Hardware:** ESP32, sensores MQ135, AHT25, BMP280, LDR, baliza WS2812, relés
- **Comunicaciones:**
  - LoRa punto a punto (433 MHz)
  - MQTT (broker en nube: `telecomunicaciones.ddns.net`)
- **Software:**
  - PlatformIO / Arduino
  - Node-RED
  - MariaDB
  - Grafana (para dashboards)
  - Frontend web con HTML/CSS/JS (detalles en carpeta `hub/frontend/`)

---

## 🛠️ Próximos pasos

- Integrar documentación de backend y frontend
- Documentar flujo de seguridad/autenticación (token en JSON)
- Incorporar esquemas gráficos de arquitectura física y lógica

---

## 📅 Estado del sistema (junio 2025)

✔ Nodos sensado y receptor funcionando  
✔ Comunicación LoRa operativa  
✔ JSON de datos y comandos implementado  
⏳ En proceso: integración con MQTT, dashboards, actuadores remotos  
📈 En desarrollo: visualización e historial de datos

