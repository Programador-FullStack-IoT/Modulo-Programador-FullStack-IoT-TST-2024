/** 
 * Controlador para manejar comandos enviados a dispositivos a través de MQTT.
 * Recibe solicitudes HTTP y publica comandos en el tópico MQTT correspondiente.
 */

// Importa el servicio MQTT para publicar mensajes
const mqttService = require('../services/mqttService');
// Importa la configuración global (incluye tópicos y claves)
const config = require('../config');

/**
 * Envía un comando a un dispositivo específico a través de MQTT.
 * @param {object} req - Objeto de solicitud de Express.
 * @param {object} res - Objeto de respuesta de Express.
 */
const sendCommand = (req, res) => {
  const { deviceId } = req.params; // Obtiene el ID del dispositivo de la URL
  const commandPayload = req.body; // Obtiene el comando del cuerpo de la petición

  // Validación: el ID del dispositivo es obligatorio
  if (!deviceId) {
    return res.status(400).json({ error: 'El ID del dispositivo es requerido.' });
  }

  // Validación: el cuerpo del comando no puede estar vacío
  if (!commandPayload || Object.keys(commandPayload).length === 0) {
    return res.status(400).json({ error: 'El cuerpo del comando no puede estar vacío.' });
  }

  // Construir el tópico de comando específico para el dispositivo
  const topic = config.mqtt.topic.command.replace('{deviceId}', deviceId); // Reemplaza el placeholder por el ID real
  const message = JSON.stringify(commandPayload); // Serializa el comando a JSON

  try {
    mqttService.publishCommand(topic, message); // Publica el comando en el broker MQTT
    res.status(200).json({ success: true, message: `Comando enviado al dispositivo ${deviceId}.` });
  } catch (error) {
    res.status(500).json({ success: false, error: 'Error interno al enviar el comando.' });
  }
};

// Exporta la función para ser utilizada en las rutas
module.exports = {
  sendCommand,
};