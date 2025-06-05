# Documentación de la API OmniSens

Esta API funciona como backend para la plataforma IoT OmniSens, gestionando la ingesta de datos desde MQTT, el almacenamiento en una base de datos MariaDB y proporcionando endpoints HTTP para la consulta de datos y el control de dispositivos.

## URL Base

La API suele estar alojada en una URL como `http://localhost:3000` (desarrollo) o `https://tu-dominio-api.com` (producción). Los endpoints descritos a continuación son relativos a esta URL base.

## Autenticación

Todos los endpoints bajo el prefijo `/api/` requieren una clave API. La clave debe enviarse en la cabecera HTTP `x-api-key`.

Ejemplo: `x-api-key: tu-clave-api-secreta`

La clave API se configura en el archivo `.env` (`API_KEY`) y se valida contra la tabla `api_users` en la base de datos.

## Comunicación MQTT

### 1. Datos Entrantes (Dispositivo a API vía MQTT)

Los dispositivos publican datos en temas MQTT específicos. La API se suscribe a estos temas para recibir y procesar los datos.

#### a. Datos de Sensores

*   **Patrón de Tema**: `omnisens/devices/{deviceId}/data`
    *   `{deviceId}` es el identificador único del dispositivo que publica.
*   **Formato**: JSON
*   **Ejemplo de Payload**:
    ```json
    {
      "timestamp": "2024-07-15T10:30:00Z",
      "temperatura1": 25.5,
      "temperatura2": 26.1,
      "uv": 5.0,
      "humedad_relativa": 45.2,
      "presion_atm": 1012.5,
      "luz": 700,
      "gas_level": "Normal",
      "codigo_alarma": "0"
    }
    ```
    *   Si se omite `timestamp`, la API asigna la hora actual del servidor al recibir el dato. Se recomienda enviar timestamps en formato ISO 8601.
*   **Acción**: La API analiza estos datos y los almacena en la tabla `sensor_data`, asociada al `deviceId`.

#### b. Estado de Actuadores/Dispositivo

*   **Patrón de Tema**: `omnisens/devices/{deviceId}/status`
*   **Formato**: JSON
*   **Propósito**: Reportar el estado actual de los actuadores o la salud general del dispositivo.
*   **Ejemplo de Payload (Estado de Actuador)**:
    ```json
    {
      "timestamp": "2024-07-15T10:32:00Z",
      "rele1_status": true,
      "rele2_status": false,
      "pwm_value": 128
    }
    ```
*   **Acción**: La API analiza esto y lo almacena en la tabla `actuator_status`.

### 2. Comandos Salientes (API a Dispositivo vía MQTT)

La API publica comandos en temas MQTT específicos por dispositivo, permitiendo el control remoto de actuadores y otras funciones.

*   **Patrón de Tema**: `omnisens/devices/{deviceId}/command`
*   **Formato**: JSON
*   **Propósito**: Enviar instrucciones de control a un dispositivo.
*   **Ejemplos de Payload**:
    *   Controlar un relé:
        ```json
        {
          "actuator": "rele1",
          "action": "set",
          "value": true
        }
        ```
    *   Establecer un valor PWM:
        ```json
        {
          "actuator": "pwm",
          "action": "set",
          "value": 200
        }
        ```
*   **Acción**: Los dispositivos suscritos a su tema de comandos recibirán y ejecutarán estos mensajes. La API registra los comandos enviados en la tabla `commands_log`.

## Estructura de la Base de Datos

La API utiliza una base de datos MariaDB (nombre por defecto `AQI_DB`). Las tablas principales incluyen:
*   `devices`: Información de cada dispositivo conectado.
*   `sensor_data`: Lecturas de sensores en serie temporal.
*   `actuator_status`: Estado de actuadores en serie temporal.
*   `commands_log`: Registro de comandos enviados a dispositivos.
*   `api_users`: Almacena claves API para autenticación.

Consulta `omnisens_schema.sql` para la definición completa del esquema.

## Endpoints de la API

### Verificación de Estado

*   **GET** `/health`
    *   Descripción: Proporciona el estado de salud de la API, incluyendo conectividad MQTT y BD.
    *   Autenticación: Ninguna.
    *   Respuesta (`200 OK`):
        ```json
        {
          "status": "UP",
          "timestamp": "2024-07-15T12:00:00.000Z",
          "mqttConnected": true,
          "dbPoolInitialized": true,
          "environment": "development"
        }
        ```

### Endpoints de Datos (Requieren Clave API)

*   **GET** `/api/devices`
    *   Descripción: Recupera la lista de todos los dispositivos registrados.
    *   Respuesta (`200 OK`): Array de objetos de dispositivos.
        ```json
        [
          { "device_id": "dev001", "name": "Sensor Array Alpha", "type": "MultiSensor", ... },
          { "device_id": "dev002", "name": "Control Unit Beta", "type": "ActuatorController", ... }
        ]
        ```

*   **GET** `/api/devices/{deviceId}/data`
    *   Descripción: Recupera el historial de datos de sensores para un dispositivo específico.
    *   Parámetros de URL:
        *   `deviceId` (string, requerido): ID del dispositivo.
    *   Parámetros de consulta:
        *   `limit` (entero, opcional, por defecto: 100): Máximo de registros.
        *   `offset` (entero, opcional, por defecto: 0): Desplazamiento para paginación.
        *   `startDate` (string, opcional, formato: `YYYY-MM-DD` o `YYYY-MM-DD HH:MM:SS`): Inicio del rango de tiempo.
        *   `endDate` (string, opcional, formato: `YYYY-MM-DD` o `YYYY-MM-DD HH:MM:SS`): Fin del rango de tiempo.
    *   Respuesta (`200 OK`): Array de objetos de datos de sensores para el dispositivo.

*   **GET** `/api/devices/{deviceId}/data/latest`
    *   Descripción: Recupera el dato de sensor más reciente para un dispositivo específico.
    *   Parámetros de URL:
        *   `deviceId` (string, requerido): ID del dispositivo.
    *   Respuesta (`200 OK`): Objeto de datos de sensor único, o `404 Not Found` si no existen datos.

### Endpoints de Comando (Requieren Clave API)

*   **POST** `/api/devices/{deviceId}/command`
    *   Descripción: Envía un comando a un dispositivo específico vía MQTT.
    *   Parámetros de URL:
        *   `deviceId` (string, requerido): ID del dispositivo a comandar.
    *   Cuerpo de la petición (JSON): El payload del comando (ver sección "Comandos Salientes" de MQTT para el formato).
        Ejemplo:
        ```json
        {
          "actuator": "rele1",
          "action": "set",
          "value": true
        }
        ```
    *   Respuesta (`202 Accepted`):
        ```json
        {
          "message": "Comando aceptado para enviar al dispositivo",
          "deviceId": "dev001",
          "command": { "actuator": "rele1", "action": "set", "value": true }
        }
        ```
    *   Respuestas de error:
        *   `400 Bad Request`: Formato de comando inválido en el cuerpo de la petición.
        *   `500 Internal Server Error`: Si la API falla al procesar o enviar el comando.
        *   `503 Service Unavailable`: Si el cliente MQTT no está conectado.

## Instalación y Ejecución de la API

1.  **Prerrequisitos**: Node.js (v16+ recomendado), npm y una instancia de MariaDB en funcionamiento.
2.  **Clonar el repositorio**: (Asumiendo que esta API es parte de un proyecto mayor)
3.  **Configurar**:
    *   Copia o renombra `.env.example` a `.env` en el directorio `api/`.
    *   Actualiza `.env` con tus credenciales de base de datos, detalles del broker MQTT y una `API_KEY` segura.
4.  **Instalar dependencias**: Navega al directorio `api/` y ejecuta `npm install`.
5.  **Configuración de la base de datos**:
    *   Conéctate a tu instancia de MariaDB.
    *   Crea la base de datos especificada en `.env` (por ejemplo, `AQI_DB`).
    *   Ejecuta el script `omnisens_schema.sql` en esta base de datos para crear las tablas necesarias.
6.  **Iniciar la API**:
    *   Para desarrollo (con recarga automática): `npm run dev`
    *   Para producción: `npm start`

## Consideraciones de Seguridad

*   **HTTPS**: Muy recomendado para producción. Configura tu proxy inverso (por ejemplo, Nginx) para manejar la terminación SSL, o modifica `server.js` para usar el módulo `https` con tus certificados SSL.
*   **Seguridad MQTT**:
    *   Usa TLS para conexiones MQTT si tu broker lo soporta (por ejemplo, `mqtts://...`).
    *   Asegúrate de que tu broker MQTT utilice autenticación y autorización robustas (ACLs). Esta API usa usuario/contraseña.
*   **Seguridad de la Base de Datos**:
    *   Protege las credenciales de la base de datos. No subas archivos `.env` con secretos de producción al control de versiones.
    *   La API usa consultas parametrizadas vía `mysql2` para prevenir inyección SQL.
*   **Gestión de Claves API**:
    *   Usa claves API fuertes y únicas.
    *   Almacena las claves API de forma segura (el ejemplo usa una tabla `api_users`).
    *   Considera políticas de rotación de claves.
    *   Para escenarios más avanzados, explora autenticación basada en OAuth2 o JWT.
*   **Validación de Entradas**: La API incluye validación básica para los payloads de comandos. Se recomienda aplicar validación exhaustiva a todas las entradas.
*   **Limitación de Tasa**: Implementa limitación de tasa en los endpoints para prevenir abusos.
*   **Registro de Logs**: Hay registro exhaustivo de mensajes MQTT, operaciones de base de datos y peticiones/errores de la API. Monitorea los logs regularmente.
*   **CORS**: Configura Cross-Origin Resource Sharing (CORS) de forma restrictiva en `app.js` para producción, permitiendo solicitudes solo desde dominios de confianza.

## Escalabilidad

*   **Diseño Stateless**: La API está diseñada para ser stateless, facilitando el escalado horizontal ejecutando múltiples instancias detrás de un balanceador de carga.
*   **Pool de Conexiones**: `mysql2` utiliza un pool de conexiones para una gestión eficiente de recursos de base de datos.
*   **Broker MQTT**: Asegúrate de que tu broker MQTT esté configurado para alta disponibilidad y pueda manejar el volumen esperado de mensajes.
*   **Base de Datos**: Optimiza las consultas y asegúrate de que tu servidor de base de datos tenga recursos suficientes.