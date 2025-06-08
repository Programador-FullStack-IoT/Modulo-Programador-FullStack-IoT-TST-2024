# Documentación de la API OmniSens

Bienvenido a la documentación oficial de la API RESTful para el proyecto OmniSens. Esta API permite la interacción con dispositivos IoT, el almacenamiento de datos de sensores y el envío de comandos a través de MQTT.

**URL Base**: `http://localhost:3000/api`

---

## Autenticación

Todas las rutas de la API están protegidas y requieren una clave de API para su uso. La clave debe ser enviada en la cabecera HTTP `x-api-key` en cada solicitud.

**Cabecera requerida:**
`x-api-key: <tu_clave_secreta_aqui>`

Si la clave no es proporcionada o es incorrecta, la API responderá con un código de estado `401 Unauthorized`.

---

## Endpoints de la API

A continuación se detallan los endpoints disponibles.

### Gestión de Dispositivos y Datos

#### 1. Listar todos los dispositivos

Recupera una lista de todos los dispositivos que han reportado datos al sistema.

* **Método:** `GET`
* **Endpoint:** `/devices`
* **Parámetros de URL:** Ninguno.
* **Parámetros de Query:** Ninguno.
* **Cuerpo de la Petición:** Ninguno.
* **Respuesta Exitosa (200 OK):**
    Una lista de objetos de dispositivo.
    ```json
    [
      {
        "device_eui": "dispositivo_lora_001",
        "name": "Dispositivo dispositivo_lora_001",
        "location": null,
        "created_at": "2024-06-08T10:30:00.000Z"
      },
      {
        "device_eui": "esp32_sensor_A",
        "name": "Dispositivo esp32_sensor_A",
        "location": "Laboratorio 1",
        "created_at": "2024-06-08T11:15:00.000Z"
      }
    ]
    ```

#### 2. Obtener datos de un dispositivo específico

Recupera las últimas mediciones de un dispositivo específico, ordenadas por fecha de forma descendente.

* **Método:** `GET`
* **Endpoint:** `/devices/:deviceId/data`
* **Parámetros de URL:**
    * `deviceId` (string, requerido): El `device_eui` del dispositivo.
* **Parámetros de Query:**
    * `limit` (integer, opcional): Número de registros a devolver. Por defecto es `20`.
* **Cuerpo de la Petición:** Ninguno.
* **Ejemplo de Petición:**
    `GET /api/devices/dispositivo_lora_001/data?limit=5`
* **Respuesta Exitosa (200 OK):**
    Una lista de objetos de mediciones.
    ```json
    [
        {
            "value": 25.5,
            "timestamp": "2024-06-08T12:05:10.000Z",
            "sensor_type": "temperature",
            "unit": "°C"
        },
        {
            "value": 45.2,
            "timestamp": "2024-06-08T12:05:10.000Z",
            "sensor_type": "humidity",
            "unit": "%"
        },
        {
            "value": 25.1,
            "timestamp": "2024-06-08T12:04:55.000Z",
            "sensor_type": "temperature",
            "unit": "°C"
        }
    ]
    ```
* **Respuesta de Error (404 Not Found):**
    Se devuelve si no se encuentran mediciones para el `deviceId` especificado.
    ```json
    {
      "message": "No se encontraron datos para el dispositivo especificado."
    }
    ```

### Gestión de Comandos

#### 3. Enviar un comando a un dispositivo

Publica un mensaje de comando en el tópico MQTT correspondiente a un dispositivo específico.

* **Método:** `POST`
* **Endpoint:** `/devices/:deviceId/command`
* **Parámetros de URL:**
    * `deviceId` (string, requerido): El `device_eui` del dispositivo que recibirá el comando.
* **Cuerpo de la Petición (JSON, requerido):**
    Un objeto JSON que representa el comando a enviar. La estructura es flexible y debe ser interpretada por el dispositivo final.
    ```json
    {
      "actuator": "led_rojo",
      "value": 1
    }
    ```
    Otro ejemplo:
    ```json
    {
        "actuator": "screen",
        "action": "display_message",
        "payload": "Alerta: Nivel de CO2 alto"
    }
    ```
* **Respuesta Exitosa (200 OK):**
    ```json
    {
      "success": true,
      "message": "Comando enviado al dispositivo dispositivo_lora_001."
    }
    ```
* **Respuesta de Error (400 Bad Request):**
    Se devuelve si falta el `deviceId` o si el cuerpo de la petición está vacío.
    ```json
    {
      "error": "El cuerpo del comando no puede estar vacío."
    }
    ```