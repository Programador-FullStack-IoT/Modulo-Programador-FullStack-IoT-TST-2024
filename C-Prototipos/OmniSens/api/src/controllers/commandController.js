/** * Controlador para manejar comandos enviados a dispositivos a través de MQTT.
 * Este módulo recibe solicitudes HTTP y envía comandos a dispositivos específicos.
 * Utiliza el servicio de MQTT para publicar los comandos en el tópico correspondiente.
 */


const mqttService = require('../services/mqttService');
const config = require('../config');

/**
 * Envía un comando a un dispositivo específico a través de MQTT.
 * @param {object} req - Objeto de solicitud de Express.
 * @param {object} res - Objeto de respuesta de Express.
 */
const sendCommand = (req, res) => {
  const { deviceId } = req.params;
  const commandPayload = req.body;

  if (!deviceId) {
    return res.status(400).json({ error: 'El ID del dispositivo es requerido.' });
  }

  if (!commandPayload || Object.keys(commandPayload).length === 0) {
    return res.status(400).json({ error: 'El cuerpo del comando no puede estar vacío.' });
  }

  // Construir el tópico de comando específico para el dispositivo
  const topic = config.mqtt.topic.command.replace('{deviceId}', deviceId);
  const message = JSON.stringify(commandPayload);

  try {
    mqttService.publishCommand(topic, message);
    res.status(200).json({ success: true, message: `Comando enviado al dispositivo ${deviceId}.` });
  } catch (error) {
    res.status(500).json({ success: false, error: 'Error interno al enviar el comando.' });
  }
};

module.exports = {
  sendCommand,
};