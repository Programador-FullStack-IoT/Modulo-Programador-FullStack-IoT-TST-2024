# API OmniSens

Esta API RESTful permite la consulta de datos y el envío de comandos a dispositivos IoT integrados en la plataforma OmniSens.

**URL Base:** `https://<host>:<puerto>/api` (Ej: `https://localhost:3000/api`)

---

## Autenticación

Todas las rutas requieren una API Key enviada en la cabecera HTTP `x-api-key`.

- **Cabecera requerida:**
  ```
  x-api-key: <tu_clave_secreta_aqui>
  ```
- Si la clave es incorrecta o falta, la API responde con `401 Unauthorized`.
- Configura tu clave en el archivo `.env` con la variable `API_KEY`.

---

## Tabla de Contenidos
- [Arquitectura General](#arquitectura-general)
- [Guía Rápida de Despliegue](#guía-rápida-de-despliegue)
- [Endpoints de Dispositivos](#endpoints-de-dispositivos-devices)
- [Ejemplos de Payloads Avanzados](#ejemplos-de-payloads-avanzados)
- [Ejemplos de Scripts de Prueba](#ejemplos-de-scripts-de-prueba)
- [Preguntas Frecuentes (FAQ)](#preguntas-frecuentes-faq)
- [Recursos y Enlaces Útiles](#recursos-y-enlaces-útiles)

---

## Arquitectura General

```mermaid
flowchart TD
    subgraph Usuario
      A1[Navegador/Cliente]
    end
    subgraph API[API OmniSens (Express + HTTPS)]
      B1[Endpoints REST]
      B2[Middleware Seguridad]
      B3[MQTT Service]
      B4[DB Service]
    end
    subgraph IoT[Dispositivos IoT]
      C1[ESP32/NodeMCU]
    end
    subgraph Broker[Broker MQTT (MQTTS)]
      D1[Broker MQTT]
    end
    subgraph DB[Base de Datos MariaDB]
      E1[Tabla Measurements]
    end
    A1-->|HTTPS|B1
    B1-->|Comandos/Consultas|B3
    B1-->|Consultas|B4
    B3-->|MQTTS|D1
    D1-->|MQTTS|C1
    C1-->|MQTTS|D1
    D1-->|MQTTS|B3
    B4-->|SQL|E1
    E1-->|Datos|B4
```

---

## Guía Rápida de Despliegue

1. **Clona el repositorio y entra a la carpeta de la API:**
   ```sh
   git clone <url-del-repo>
   cd C-Prototipos/OmniSens/API\ (nueva)
   ```
2. **Instala las dependencias:**
   ```sh
   npm install
   ```
3. **Configura el archivo `.env`:**
   - Copia el ejemplo y ajusta las variables según tu entorno (broker, DB, etc).
4. **Agrega los certificados SSL en la carpeta `certs/`:**
   - `key.pem` y `cert.pem` para el servidor HTTPS.
   - `broker-ca.pem` para la conexión MQTTS.
5. **Inicia el servidor:**
   ```sh
   npm start
   ```
6. **Prueba la API:**
   - Accede a `https://localhost:3000/api/devices` para verificar funcionamiento.

---

## Endpoints de Dispositivos (`/devices`)

### 1. Listar todos los dispositivos

- **GET** `/devices`
- Devuelve un array de objetos con los `device_id` únicos.

### 2. Obtener datos de un dispositivo específico

- **GET** `/devices/:deviceId/data?limit=N`
- Devuelve los últimos N registros de medición para el dispositivo.

### 3. Enviar un comando a un dispositivo

- **POST** `/devices/:deviceId/command`
- Cuerpo JSON:
  ```json
  {
    "actuator": "led_azul",
    "value": 1
  }
  ```

---

## Ejemplos de Payloads Avanzados

### Ejemplo de datos recibidos de un dispositivo
```json
{
  "temperature": 23.7,
  "humidity": 58.2,
  "co2": 420,
  "pm25": 12.5,
  "voc": 0.03,
  "battery": 87
}
```

### Ejemplo de comando avanzado
```json
{
  "actuator": "ventilador",
  "value": "auto",
  "params": {
    "minTemp": 22,
    "maxTemp": 28,
    "duration": 300
  }
}
```

---

## Ejemplos de Scripts de Prueba

### Usando `curl`

- **Listar dispositivos:**
  ```sh
  curl -k -H "x-api-key: <tu_clave_secreta_aqui>" https://localhost:3000/api/devices
  ```
- **Obtener datos de un dispositivo:**
  ```sh
  curl -k -H "x-api-key: <tu_clave_secreta_aqui>" "https://localhost:3000/api/devices/esp32-01/data?limit=5"
  ```
- **Enviar comando:**
  ```sh
  curl -k -X POST https://localhost:3000/api/devices/esp32-01/command \
    -H "x-api-key: <tu_clave_secreta_aqui>" \
    -H "Content-Type: application/json" \
    -d '{"actuator":"led_azul","value":1}'
  ```

### Usando Node.js (axios)

```js
const axios = require('axios');
process.env.NODE_TLS_REJECT_UNAUTHORIZED = '0'; // Solo para pruebas locales

axios.post('https://localhost:3000/api/devices/esp32-01/command', {
  actuator: 'ventilador',
  value: 'auto',
  params: { minTemp: 22, maxTemp: 28, duration: 300 }
}, {
  headers: { 'x-api-key': '<tu_clave_secreta_aqui>' }
})
.then(res => console.log(res.data))
.catch(err => console.error(err.response?.data || err));
```

---

## Preguntas Frecuentes (FAQ)

**¿Qué pasa si un dispositivo no responde a un comando?**
- La API solo garantiza que el comando fue publicado en el broker MQTT. No puede asegurar la ejecución en el dispositivo.

**¿Cómo agrego nuevos sensores o campos?**
- Asegúrate de que la tabla `Measurements` en la base de datos tenga las columnas necesarias y que el firmware del dispositivo envíe los datos en el JSON.

**¿Puedo usar HTTP en vez de HTTPS?**
- No, por seguridad la API solo expone el puerto HTTPS.

**¿Cómo puedo ver los logs de la API?**
- Los logs se muestran en la consola donde se ejecuta el servidor. Para producción, se recomienda integrar un sistema de logging.

**¿Qué hago si la API no conecta con el broker o la base de datos?**
- Verifica las variables de entorno, la red y los certificados. Consulta los mensajes de error en la consola.

---

## Recursos y Enlaces Útiles
- [Documentación MQTT](https://mqtt.org/documentation)
- [Express.js](https://expressjs.com/)
- [MariaDB](https://mariadb.org/)
- [Mermaid Live Editor](https://mermaid.live/) (para visualizar diagramas)

---