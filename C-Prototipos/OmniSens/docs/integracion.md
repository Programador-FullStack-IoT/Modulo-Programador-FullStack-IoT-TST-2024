# Integración del Sistema OmniSens

Este documento describe la integración completa del sistema IoT de monitoreo ambiental desarrollado por el grupo AQI – PLATA, desde los sensores hasta el dashboard final.

---

## 🧱 Componentes Integrados

| Componente                 | Función principal                                      |
|----------------------------|--------------------------------------------------------|
| Nodo de Sensado            | Captura variables ambientales y genera JSON            |
| Nodo Receptor              | Recibe LoRa y publica por MQTT                         |
| Nodo Actuador              | Recibe comandos por LoRa/MQTT y activa salidas         |
| Broker MQTT (Mosquitto)    | Comunicación entre microcontroladores y backend        |
| Node-RED                   | Procesa, enruta y almacena datos                       |
| MariaDB                    | Almacenamiento persistente de datos                    |
| Dashboard (Grafana/UI)     | Visualización en tiempo real y control manual          |

---

## 🔄 Flujo completo de datos y control

1. **Sensado:**
   - El nodo sensado mide: temperatura, humedad, gases, presión, luz.
   - Genera un mensaje JSON y lo envía por LoRa al nodo receptor.

2. **Recepción y publicación MQTT:**
   - El nodo receptor procesa el JSON y publica cada variable en un topic MQTT.
   - MQTT Broker está en la nube: `telecomunicaciones.ddns.net:2480`.

3. **Procesamiento:**
   - Node-RED está suscripto a los topics del broker.
   - Muestra en tiempo real los datos y los guarda en MariaDB.
   - Permite generar reglas de control automático (por ejemplo, si gas > 400 → activar ventilador).

4. **Control:**
   - Desde Node-RED UI o Grafana, se pueden enviar comandos al nodo actuador.
   - Node-RED publica un JSON en un topic específico (`grupoAQI/ordenes`).
   - El nodo actuador ejecuta los comandos (reles, PWM, baliza).

---

## 🌐 Detalles de comunicación

### MQTT

| Parámetro   | Valor                            |
|-------------|----------------------------------|
| Broker      | `telecomunicaciones.ddns.net`    |
| Puerto MQTT | `2480`                           |
| Usuario     | `telecomunicaciones`            |
| Contraseña  | `cohorte*2024`                  |

### LoRa

- Banda: 433 MHz
- Comunicación punto a punto entre nodos ESP32

---

## 🧪 Ejemplo de flujo (escenario real)

1. MQ135 detecta gas → genera JSON con valor alto
2. JSON llega al nodo receptor → publica `grupoAQI/gas`
3. Node-RED detecta que `gas > 400` → publica comando en `grupoAQI/ordenes`
4. Nodo actuador recibe el JSON → activa ventilador por PWM + baliza LED

---

## 🛠️ Requisitos de integración local

Para reproducir todo el entorno en local (fuera de la nube), se necesita:

- Mosquitto Broker en puerto 1883
- Node-RED con acceso a la red
- MariaDB en localhost:3306 con tabla de datos definida
- Copia de los `.ino` o proyectos PlatformIO en nodos

---

## 🧩 Integración en contenedores (Docker)

Los siguientes servicios están disponibles en la nube:

| Servicio       | URL                                             |
|----------------|--------------------------------------------------|
| Portainer      | http://telecomunicaciones.ddns.net:8080/portainer |
| Node-RED       | http://telecomunicaciones.ddns.net:8080/nodered |
| Node-RED UI    | http://telecomunicaciones.ddns.net:8080/nodered/ui |
| Grafana        | http://telecomunicaciones.ddns.net:8080/grafana  |
| MariaDB        | telecomunicaciones.ddns.net:6034 (acceso por gestor externo) |

---

## 📎 Archivos relacionados

- [`nodo-sensado.md`](./nodo-sensado.md)
- [`nodo-receptor.md`](./nodo-receptor.md)
- [`nodo-actuador.md`](./nodo-actuador.md)
- [`arquitectura.md`](./arquitectura.md)

---

## ✅ Estado actual (junio 2025)

✔ Comunicación entre nodos implementada  
✔ MQTT operativo y testeado  
✔ Node-RED suscripto a topics 
✔ Dashboard con control parcial 
 


