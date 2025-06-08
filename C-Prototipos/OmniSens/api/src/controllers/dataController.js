/** 
 * Controlador para manejar los datos entrantes de los sensores.
 * Procesa mensajes recibidos desde MQTT y los valida antes de almacenarlos.
 */

// Importa el servicio de base de datos para guardar lecturas
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
  const topicParts = topic.split('/'); // Divide el tópico por /
  const deviceId = topicParts.length > 1 ? topicParts[1] : null; // Obtiene el segundo elemento como ID

  // Validación: si no se puede extraer el ID, muestra error
  if (!deviceId) {
    console.error('No se pudo extraer el ID del dispositivo del tópico.');
    return;
  }

  try {
    // Convertir el payload (Buffer) a un string y luego a JSON
    const message = JSON.parse(payload.toString()); // Intenta parsear el mensaje
    console.log(`Payload (JSON válido) para ${deviceId}:`, message);

    // Aquí se podría guardar en la base de datos usando dbService.saveSensorData(deviceId, message);

  } catch (error) {
    // Si el payload no es JSON válido, muestra error
    console.error(`Error al procesar el mensaje del tópico ${topic}: No es un JSON válido.`, error.message);
  }
};

// Exporta la función para ser utilizada por el servicio MQTT
module.exports = {
  handleIncomingMqttData,
};