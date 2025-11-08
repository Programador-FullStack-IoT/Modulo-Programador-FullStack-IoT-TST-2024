// Importamos dotenv para cargar las variables de entorno desde un archivo .env
require('dotenv').config();

// Configuración centralizada de la aplicación.
// Aquí se definen los parámetros de conexión para la API, MQTT y la base de datos.
const config = {
  api: { 
    port: process.env.API_PORT || 3000, // Puerto en el que se ejecuta la API
  },
  mqtt: {
    brokerUrl: process.env.MQTT_BROKER_URL, // URL del broker MQTT
    port: process.env.MQTT_PORT, // Puerto del broker MQTT
    username: process.env.MQTT_USERNAME, // Usuario para autenticación MQTT
    password: process.env.MQTT_PASSWORD, // Contraseña para autenticación MQTT
    // Convierte la string de tópicos en un array, eliminando espacios en blanco.
    subscribeTopics: process.env.MQTT_SUBSCRIBE_TOPICS
      ? process.env.MQTT_SUBSCRIBE_TOPICS.split(',').map(topic => topic.trim())
      : [], // Devuelve un array vacío si la variable no está definida
  },
  database: {
    host: process.env.DB_HOST, // Host de la base de datos
    port: process.env.DB_PORT, // Puerto de la base de datos
    database: process.env.DB_DATABASE, // Nombre de la base de datos
    user: process.env.DB_USER, // Usuario de la base de datos
    password: process.env.DB_PASSWORD, // Contraseña de la base de datos
  },
};

module.exports = config;