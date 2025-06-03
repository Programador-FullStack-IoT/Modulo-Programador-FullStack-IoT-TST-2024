const dbService = require('../services/dbService');

async function getDeviceData(req, res) {
  try {
    const { deviceId } = req.params;
    const { limit, offset, startDate, endDate } = req.query;
    const data = await dbService.getSensorData(deviceId, { limit, offset, startDate, endDate });
    res.json(data);
  } catch (error) {
    console.error('Error in getDeviceData controller:', error.message);
    res.status(500).json({ error: 'Failed to retrieve device data' });
  }
}

/**
 * Controlador para obtener los datos más recientes de un dispositivo específico.
 *
 * @async
 * @function
 * @param {import('express').Request} req - Objeto de solicitud de Express, debe contener el parámetro deviceId.
 * @param {import('express').Response} res - Objeto de respuesta de Express.
 * @returns {Promise<void>} Devuelve una respuesta JSON con los datos más recientes del dispositivo o un mensaje de error.
 */
async function getLatestDeviceData(req, res) {
  try {
    const { deviceId } = req.params;
    const data = await dbService.getLatestSensorData(deviceId);
    if (data) {
      res.json(data);
    } else {
      res.status(404).json({ message: `No data found for device ${deviceId}.` });
    }
  } catch (error) {
    console.error('Error in getLatestDeviceData controller:', error.message);
    res.status(500).json({ error: 'Failed to retrieve latest device data' });
  }
}

async function listDevices(req, res) {
  try {
    const devices = await dbService.getDevices();
    res.json(devices);
  } catch (error) {
    console.error('Error in listDevices controller:', error.message);
    res.status(500).json({ error: 'Failed to retrieve devices' });
  }
}

module.exports = {
  getDeviceData,
  getLatestDeviceData,
  listDevices,
};