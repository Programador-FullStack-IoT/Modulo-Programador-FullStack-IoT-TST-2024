
const mqtt = require('mqtt');
const fs = require('fs');
const config = require('../config');
const dbService = require('./databaseService'); // <--- 1. Importar el servicio de DB

// Opciones de conexión para MQTTS
const options = {
  port: config.mqtt.port,
  username: config.mqtt.username,
  password: config.mqtt.password,
  // Opciones TLS/SSL
  // rejectUnauthorized: false, // Poner en 'false' solo para pruebas con certificados autofirmados
  ca: fs.readFileSync('./certs/broker-ca.pem'), // Certificado de la Autoridad Certificadora del broker
};

const client = mqtt.connect(config.mqtt.brokerUrl, options);

const MQTT_TOPIC_DATA = 'devices/+/data';

client.on('connect', () => {
  console.log('✅ Conectado exitosamente al Broker MQTT');
  
  if (config.mqtt.subscribeTopics && config.mqtt.subscribeTopics.length > 0) {
    // Suscribirse a cada tópico definido en la configuración
    client.subscribe(config.mqtt.subscribeTopics, (err, granted) => {
      if (err) {
        console.error('❌ Error al suscribirse a los tópicos:', err);
        return;
      }
      granted.forEach(sub => {
        console.log(`📡 Suscrito exitosamente al tópico: "${sub.topic}" con QoS ${sub.qos}`);
      });
    });
  } else {
    console.warn('⚠️ No hay tópicos MQTT definidos para suscribirse en la configuración.');
  }
});

client.on('message', async (topic, message) => { // <--- 2. Convertir la función a 'async'
  console.log(`📥 Mensaje recibido en el tópico: ${topic}`);
  
  // Extraer el deviceId del tópico. Ej: "devices/device01/data" -> "device01"
  const deviceId = topic.split('/')[1];

  try {
    // Validar que el mensaje es un JSON
    const data = JSON.parse(message.toString());
    console.log(`📊 Datos recibidos de ${deviceId}:`, data);

    // 3. Llamar a la función para insertar en la base de datos
    await dbService.insertMeasurement(deviceId, data);

  } catch (error) {
    console.error(`❌ Error al procesar el mensaje de ${deviceId}: No es un JSON válido.`, message.toString());
  }
});

client.on('error', (err) => {
  console.error('❌ Error en el cliente MQTT:', err);
});

client.on('close', () => {
  console.log('🔌 Cliente MQTT desconectado.');
});

module.exports = client;

/**
 * Publica un mensaje en un tópico MQTT específico.
 * @param {string} topic - El tópico al que se publicará el mensaje.
 * @param {string} message - El mensaje a publicar.
 */
function publishMessage(topic, message) {
  client.publish(topic, message, { qos: 1 }, (err) => { // Usamos QoS 1 para asegurar la entrega
    if (err) {
      console.error(`❌ Error al publicar en el tópico ${topic}:`, err);
    } else {
      console.log(`✅ Mensaje publicado en el tópico: ${topic}`);
      console.log(`- Mensaje: ${message}`);
    }
  });
}

// Exportamos tanto el cliente como la nueva función
module.exports = {
  client,
  publishMessage, // <--- EXPORTAMOS LA NUEVA FUNCIÓN
};