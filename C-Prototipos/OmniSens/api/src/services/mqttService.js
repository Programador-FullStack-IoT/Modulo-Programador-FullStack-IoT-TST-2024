// Servicio para la gestión de la comunicación MQTT
// Configura la conexión, suscripción y publicación de mensajes con el broker MQTT

const mqtt = require('mqtt'); // Importa la librería MQTT
const config = require('../config'); // Importa la configuración global
const dataController = require('../controllers/dataController'); // Importa el controlador de datos

let client; // Cliente MQTT global

// Función para conectar al broker MQTT y suscribirse a los tópicos necesarios
const connect = () => {
  console.log(`Conectando al broker MQTT en ${config.mqtt.brokerUrl}...`);
  
  client = mqtt.connect(config.mqtt.brokerUrl, config.mqtt.options); // Crea la conexión MQTT

  client.on('connect', () => {
    console.log('¡Conectado exitosamente al broker MQTT!');
    // Suscribirse al tópico de datos de todos los dispositivos
    client.subscribe(config.mqtt.topic.data, (err) => {
      if (!err) {
        console.log(`Suscrito al tópico: ${config.mqtt.topic.data}`);
      } else {
        console.error('Error al suscribirse:', err);
      }
    });
  });

  // Maneja la recepción de mensajes MQTT
  client.on('message', (topic, payload) => {
    // Cuando llega un mensaje, lo pasamos al controlador de datos
    dataController.handleIncomingMqttData(topic, payload);
  });

  // Maneja errores de conexión
  client.on('error', (err) => {
    console.error('Error en el cliente MQTT:', err);
    client.end();
  });

  client.on('reconnect', () => {
    console.log('Reconectando al broker MQTT...');
  });

  client.on('close', () => {
    console.log('Conexión MQTT cerrada.');
  });
};

/**
 * Publica un mensaje en un tópico MQTT específico.
 * @param {string} topic - El tópico al que se publicará el mensaje.
 * @param {string} message - El mensaje a publicar.
 */
const publishCommand = (topic, message) => {
  if (client && client.connected) {
    console.log(`Publicando comando en el tópico: ${topic}`);
    client.publish(topic, message, { qos: 1 }, (err) => { // QoS 1 para asegurar la entrega
      if (err) {
        console.error('Error al publicar el comando:', err);
      } else {
        console.log('Comando publicado exitosamente.');
      }
    });
  } else {
    console.error('No se puede publicar. Cliente MQTT no conectado.');
  }
};

// Devuelve la instancia del cliente MQTT (para pruebas o diagnósticos)
const getClient = () => client;

// Exporta las funciones principales del servicio MQTT
module.exports = {
  connect,
  getClient,
  publishCommand,
};