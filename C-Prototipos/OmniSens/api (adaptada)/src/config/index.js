// Configuración principal de la API y servicios
// Carga variables de entorno y exporta la configuración para la API, MQTT y otros módulos

require('dotenv').config(); // Carga las variables de entorno desde el archivo .env

module.exports = {
  // Configuración de la API REST (puerto y clave de acceso)
  api: {
    port: process.env.API_PORT || 3000, // Puerto en el que se ejecuta la API
    apiKey: process.env.API_KEY,        // Clave de autenticación para las rutas protegidas
  },
  // Configuración de la conexión MQTT
  mqtt: {
    brokerUrl: process.env.MQTT_BROKER_URL, // URL del broker MQTT
    options: {
      username: process.env.MQTT_USERNAME,   // Usuario para autenticación MQTT
      password: process.env.MQTT_PASSWORD,   // Contraseña para autenticación MQTT
      clientId: `omnisens_api_${Math.random().toString(16).slice(2, 8)}`, // ID único para el cliente MQTT
    },
    topic: {
      data: process.env.MQTT_TOPIC_DATA || 'devices/+/data', // Tópico para recibir datos de sensores
      command: process.env.MQTT_TOPIC_COMMAND || 'devices/{deviceId}/command', // Tópico para enviar comandos
    }
  },
  // Aquí se podría agregar la configuración de la base de datos si es necesario
};