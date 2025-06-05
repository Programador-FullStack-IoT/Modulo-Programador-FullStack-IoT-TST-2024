// Carga las variables de entorno desde un archivo .env ubicado dos niveles arriba del directorio actual
require('dotenv').config({ path: require('path').resolve(__dirname, '../../.env') });

// Define un objeto de configuración con diferentes parámetros leídos de las variables de entorno
const config = {
  // Entorno de ejecución (por ejemplo: 'development', 'production')
  env: process.env.NODE_ENV || 'development',
  // Puerto en el que correrá la aplicación, por defecto 3000 si no está definido
  port: process.env.PORT || 3000,
  // Configuración de la base de datos
  db: {
    host: process.env.DB_HOST, // Host de la base de datos
    port: parseInt(process.env.DB_PORT, 10), // Puerto de la base de datos (convertido a número)
    user: process.env.DB_USER, // Usuario de la base de datos
    password: process.env.DB_PASSWORD, // Contraseña de la base de datos
    name: process.env.DB_NAME, // Nombre de la base de datos
  },
  // Configuración para la conexión MQTT
  mqtt: {
    brokerUrl: process.env.MQTT_BROKER_URL, // URL del broker MQTT
    username: process.env.MQTT_USERNAME, // Usuario MQTT
    password: process.env.MQTT_PASSWORD, // Contraseña MQTT
    dataTopicPattern: process.env.MQTT_DATA_TOPIC_PATTERN || 'omnisens/devices/+/data', // Patrón de tópico para datos
    statusTopicPattern: process.env.MQTT_STATUS_TOPIC_PATTERN || 'omnisens/devices/+/status', // Patrón de tópico para estados
    commandTopicPrefix: process.env.MQTT_COMMAND_TOPIC_PREFIX || 'omnisens/devices/', // Prefijo de tópico para comandos
  },
  // Clave de API para autenticación o autorización
  apiKey: process.env.API_KEY,
};

// Exporta el objeto de configuración para que pueda ser usado en otras partes de la aplicación
module.exports = config;