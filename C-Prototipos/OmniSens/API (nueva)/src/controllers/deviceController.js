// Importamos el servicio MQTT y el servicio de base de datos
const mqttService = require('../service/mqttService');
const dbService = require('../service/databaseService'); // <--- Importamos el servicio de DB

/**
 * Procesa una petición para enviar un comando a un dispositivo.
 * Valida el cuerpo de la petición y publica el comando en el tópico MQTT correspondiente.
 */
async function sendCommand(req, res) {
  const { deviceId } = req.params;
  const command = req.body;

  // Validación básica del cuerpo de la petición
  if (!command || typeof command.actuator !== 'string' || typeof command.value === 'undefined') {
    return res.status(400).json({ 
      error: 'Cuerpo de la petición inválido. Se requiere un objeto con "actuator" (string) y "value".' 
    });
  }

  // Construir el tópico MQTT para el comando
  const topic = `devices/${deviceId}/command`;
  const message = JSON.stringify(command);

  try {
    // Usar el servicio MQTT para publicar el comando
    mqttService.publishMessage(topic, message);
    
    res.status(200).json({ 
      message: `Comando enviado exitosamente a ${deviceId}.`,
      topic: topic,
      command: command 
    });
  } catch (error) {
    console.error('❌ Error al intentar publicar el comando MQTT:', error);
    res.status(500).json({ error: 'Error interno del servidor al enviar el comando.' });
  }
}

/**
 * Procesa la petición para listar todos los dispositivos únicos.
 * Llama al servicio de base de datos para obtener los device_id distintos.
 */
async function listDevices(req, res, next) {
  try {
    const devices = await dbService.getAllDevices();
    res.status(200).json(devices);
  } catch (error) {
    res.status(500).json({ error: 'Error interno del servidor al consultar los dispositivos.' });
  }
}

/**
 * Procesa la petición para obtener los datos de un dispositivo específico.
 * Permite limitar la cantidad de registros devueltos mediante un parámetro opcional.
 */
async function getDeviceData(req, res, next) {
  try {
    const { deviceId } = req.params;
    const { limit } = req.query; // El límite es opcional, ej: ?limit=20
    
    const data = await dbService.getDeviceDataById(deviceId, limit);
    
    if (data.length === 0) {
      return res.status(404).json({ message: `No se encontraron datos para el dispositivo ${deviceId}.` });
    }
    
    res.status(200).json(data);
  } catch (error) {
    res.status(500).json({ error: 'Error interno del servidor al consultar los datos del dispositivo.' });
  }
}

// Exportamos los controladores para ser usados en las rutas
module.exports = {
  sendCommand,
  listDevices,
  getDeviceData,
};