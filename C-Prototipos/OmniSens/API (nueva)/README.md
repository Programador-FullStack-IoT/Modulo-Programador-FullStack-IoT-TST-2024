# Documentación de la API OmniSens

Esta API RESTful permite la consulta de datos y el envío de comandos a dispositivos IoT integrados en la plataforma OmniSens.

**URL Base:** `http://<host>:<puerto>/api` (Ej: `http://localhost:3000/api`)

---

## Endpoints de Dispositivos (`/devices`)

### 1. Listar todos los dispositivos

Obtiene una lista de todos los identificadores (`device_id`) únicos que han enviado datos al sistema.

* **Endpoint:** `GET /devices`
* **Método:** `GET`
* **Parámetros de URL:** Ninguno.
* **Cuerpo de la Petición:** Ninguno.

* **Respuesta Exitosa (Código `200 OK`)**

    Devuelve un array de objetos, cada uno conteniendo el `device_id` de un dispositivo.

    ```json
    [
      {
        "device_id": "esp32-01"
      },
      {
        "device_id": "nodemcu-A4"
      },
      {
        "device_id": "sensor-patio"
      }
    ]
    ```

* **Respuestas de Error**
    * **Código `500 Internal Server Error`**: Si ocurre un problema al consultar la base de datos.
        ```json
        {
          "success": false,
          "error": "Ha ocurrido un error interno en el servidor."
        }
        ```

---

### 2. Obtener datos de un dispositivo específico

Recupera los últimos registros de mediciones para un dispositivo específico.

* **Endpoint:** `GET /devices/:deviceId/data`
* **Método:** `GET`
* **Parámetros de URL:**
    * `deviceId` (string, **requerido**): El identificador único del dispositivo.

* **Parámetros de Query:**
    * `limit` (number, *opcional*): El número máximo de registros a devolver. Si no se especifica, por defecto es `10`.
        * Ejemplo: `/api/devices/esp32-01/data?limit=5`

* **Cuerpo de la Petición:** Ninguno.

* **Respuesta Exitosa (Código `200 OK`)**

    Devuelve un array de objetos, donde cada objeto es un registro de medición.

    ```json
    [
      {
        "measurement_id": 101,
        "device_id": "esp32-01",
        "temperature": 24.5,
        "humidity": 55.2,
        "co2": 450,
        "timestamp": "2025-06-08T10:30:00.000Z"
      },
      {
        "measurement_id": 100,
        "device_id": "esp32-01",
        "temperature": 24.6,
        "humidity": 55.1,
        "co2": 452,
        "timestamp": "2025-06-08T10:29:00.000Z"
      }
    ]
    ```

* **Respuestas de Error**
    * **Código `404 Not Found`**: Si no se encuentran datos para el `deviceId` especificado.
        ```json
        {
          "message": "No se encontraron datos para el dispositivo esp32-01."
        }
        ```
    * **Código `500 Internal Server Error`**: Si ocurre un problema al consultar la base de datos.

---

### 3. Enviar un comando a un dispositivo

Publica un mensaje de comando en el tópico MQTT correspondiente a un dispositivo para que este lo ejecute.

* **Endpoint:** `POST /devices/:deviceId/command`
* **Método:** `POST`
* **Parámetros de URL:**
    * `deviceId` (string, **requerido**): El identificador único del dispositivo al que se enviará el comando.

* **Cuerpo de la Petición (JSON, requerido)**

    Debe contener un objeto JSON con las claves `actuator` y `value`.

    ```json
    {
      "actuator": "led_azul",
      "value": 1
    }
    ```
    * `actuator` (string): El nombre del actuador a controlar.
    * `value` (any): El valor o estado a enviar al actuador (puede ser un número, booleano, string, etc.).

* **Respuesta Exitosa (Código `200 OK`)**

    Confirma que el comando fue enviado a la cola de MQTT. No garantiza que el dispositivo lo haya recibido o ejecutado.

    ```json
    {
      "message": "Comando enviado exitosamente a esp32-01.",
      "topic": "devices/esp32-01/command",
      "command": {
        "actuator": "led_azul",
        "value": 1
      }
    }
    ```

* **Respuestas de Error**
    * **Código `400 Bad Request`**: Si el cuerpo de la petición no es un JSON válido o le faltan las propiedades requeridas.
        ```json
        {
          "error": "Cuerpo de la petición inválido. Se requiere un objeto con \\"actuator\\" (string) y \\"value\\"."
        }
        ```
    * **Código `500 Internal Server Error`**: Si ocurre un problema al publicar en el broker MQTT.