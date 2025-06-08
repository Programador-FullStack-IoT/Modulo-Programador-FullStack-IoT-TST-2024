// Este módulo se encarga de procesar los datos crudos recibidos desde el broker MQTT.
// Este archivo es parte del controlador de datos de la API OmniSens.
// Importamos el servicio de base de datos (lo usaremos en el siguiente apartado)
const dbService = require('../services/dbService');

/**
 * Maneja los datos entrantes desde MQTT.
 * Valida, procesa y guarda los datos en la base de datos.
 * @param {string} topic - El tópico del que proviene el mensaje.
 * @param {Buffer} payload - El contenido del mensaje.
 */
const handleIncomingMqttData = async (topic, payload) => {
  console.log(`Mensaje recibido en el tópico: ${topic}`);
  
  const topicParts = topic.split('/');
  const deviceId = topicParts.length > 1 ? topicParts[1] : null;

  if (!deviceId) {
    console.error('No se pudo extraer el ID del dispositivo del tópico.');
    return;
  }

  try {
    const message = JSON.parse(payload.toString());
    console.log(`Payload (JSON válido) para ${deviceId}:`, message);

    // Llamamos al servicio de la base de datos para guardar los datos.
    await dbService.saveSensorData(deviceId, message);

  } catch (error) {
    // Si el payload no es un JSON válido o hay un error en la DB, lo capturamos aquí.
    console.error(`Error al procesar el mensaje para ${deviceId}:`, error.message);
  }
};

module.exports = {
  handleIncomingMqttData,
};