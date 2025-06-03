// Importa el servicio MQTT que se encargará de publicar los comandos
const mqttService = require('../services/mqttService');

// Controlador para enviar un comando a un dispositivo
async function sendDeviceCommand(req, res) {
  try {
    // Obtiene el ID del dispositivo desde los parámetros de la ruta
    const { deviceId } = req.params;
    // Obtiene el comando desde el cuerpo de la petición
    const command = req.body;

    // Valida que el comando tenga el formato correcto
    if (!command || typeof command !== 'object' || !command.actuator || !command.action) {
      return res.status(400).json({ error: 'Formato de comando inválido. Los campos "actuator" y "action" son requeridos en el cuerpo JSON.' });
    }

    // Publica el comando al dispositivo usando el servicio MQTT
    mqttService.publishCommand(deviceId, command);
    
    // Responde con un estado 202 indicando que el comando fue aceptado
    res.status(202).json({ message: 'Comando aceptado para enviar al dispositivo', deviceId, command });
  } catch (error) {
    // Muestra el error en consola para depuración
    console.error('Error en el controlador sendDeviceCommand:', error.message);
    // Maneja el caso en que el cliente MQTT no está conectado
    if (error.message === 'MQTT client not connected.') {
        res.status(503).json({ error: 'Servicio no disponible: cliente MQTT no conectado.' });
    } else {
        // Maneja otros errores generales
        res.status(500).json({ error: 'Error al enviar el comando' });
    }
  }
}

// Exporta el controlador para ser utilizado en otras partes de la aplicación
module.exports = {
  sendDeviceCommand,
};