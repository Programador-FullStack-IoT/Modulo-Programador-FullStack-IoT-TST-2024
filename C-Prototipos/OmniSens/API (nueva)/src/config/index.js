// Importamos dotenv para cargar las variables de entorno desde un archivo .env
require('dotenv').config();


const config = {
  api: { 
    port: process.env.API_PORT || 3000,
  },
  mqtt: {
    brokerUrl: process.env.MQTT_BROKER_URL,
    port: process.env.MQTT_PORT,
    username: process.env.MQTT_USERNAME,
    password: process.env.MQTT_PASSWORD,
    // Convierte la string de tópicos en un array, eliminando espacios en blanco.
    subscribeTopics: process.env.MQTT_SUBSCRIBE_TOPICS
      ? process.env.MQTT_SUBSCRIBE_TOPICS.split(',').map(topic => topic.trim())
      : [], // Devuelve un array vacío si la variable no está definida
  },
  database: {
    host: process.env.DB_HOST,
    port: process.env.DB_PORT,
    database: process.env.DB_DATABASE,
    user: process.env.DB_USER,
    password: process.env.DB_PASSWORD,
  },
};

module.exports = config;