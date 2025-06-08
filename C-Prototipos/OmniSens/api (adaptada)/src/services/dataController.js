// Servicio encargado de procesar y almacenar los datos recibidos desde MQTT
// Utiliza el servicio de base de datos para guardar las lecturas de sensores

const dbService = require('../services/dbService'); // Importa el servicio de base de datos

/**
 * Maneja los datos entrantes desde MQTT.
 * Valida, procesa y guarda los datos en la base de datos.
 * @param {string} topic - El tópico del que proviene el mensaje.
 * @param {Buffer} payload - El contenido del mensaje.
 */
const handleIncomingMqttData = async (topic, payload) => {
  console.log(`Mensaje recibido en el tópico: ${topic}`);
  
  // Extrae el deviceId del tópico (ejemplo: "devices/1234/data" -> "1234")
  const topicParts = topic.split('/');
  const deviceId = topicParts.length > 1 ? topicParts[1] : null;

  // Si no se puede extraer el ID, muestra error y termina
  if (!deviceId) {
    console.error('No se pudo extraer el ID del dispositivo del tópico.');
    return;
  }

  try {
    // Intenta parsear el payload a JSON
    const message = JSON.parse(payload.toString());
    console.log(`Payload (JSON válido) para ${deviceId}:`, message);

    // Guarda los datos en la base de datos usando el servicio
    await dbService.saveSensorData(deviceId, message);

  } catch (error) {
    // Si el payload no es JSON válido o hay error en la DB, lo muestra
    console.error(`Error al procesar el mensaje para ${deviceId}:`, error.message);
  }
};

// Exporta la función para ser utilizada por otros módulos
module.exports = {
  handleIncomingMqttData,
};