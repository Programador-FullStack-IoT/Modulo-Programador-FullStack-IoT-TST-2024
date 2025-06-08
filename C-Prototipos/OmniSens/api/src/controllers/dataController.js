/** * Controlador para manejar los datos entrantes de los sensores.
 * Este módulo procesa los mensajes recibidos desde el broker MQTT y los valida.
 * Luego, invoca al servicio de base de datos para almacenar las lecturas de los sensores.
 */


const dbService = require('../services/dbService');

/**
 * Maneja los datos entrantes desde MQTT.
 * Por ahora, solo valida y muestra los datos en consola.
 * @param {string} topic - El tópico del que proviene el mensaje.
 * @param {Buffer} payload - El contenido del mensaje.
 */
const handleIncomingMqttData = (topic, payload) => {
  console.log(`Mensaje recibido en el tópico: ${topic}`);
  
  // Extraer el deviceId del tópico (ej. de "devices/1234/data" extrae "1234")
  const topicParts = topic.split('/');
  const deviceId = topicParts.length > 1 ? topicParts[1] : null;

  if (!deviceId) {
    console.error('No se pudo extraer el ID del dispositivo del tópico.');
    return;
  }

  try {
    // Convertir el payload (Buffer) a un string y luego a JSON
    const message = JSON.parse(payload.toString());
    console.log(`Payload (JSON válido) para ${deviceId}:`, message);

    // Módulo 2: Aquí irá la lógica para guardar en la base de datos.
    // Ejemplo: dbService.saveSensorData(deviceId, message);

  } catch (error) {
    console.error(`Error al procesar el mensaje del tópico ${topic}: No es un JSON válido.`, error.message);
  }
};

module.exports = {
  handleIncomingMqttData,
};