
require('dotenv').config();

module.exports = {
  // ... (config api y db) ...
  api: {
    port: process.env.API_PORT || 3000,
    apiKey: process.env.API_KEY, // <-- Añadido
  },
  mqtt: {
    brokerUrl: process.env.MQTT_BROKER_URL,
    options: {
      username: process.env.MQTT_USERNAME,
      password: process.env.MQTT_PASSWORD,
      clientId: `omnisens_api_${Math.random().toString(16).slice(2, 8)}`,
    },
    topic: {
      data: process.env.MQTT_TOPIC_DATA || 'devices/+/data',
      // Añadir la siguiente línea
      command: process.env.MQTT_TOPIC_COMMAND || 'devices/{deviceId}/command',
    }
  },
};