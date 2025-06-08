const express = require('express');
const router = express.Router();
const deviceController = require('../controllers/deviceController');
const { apiKeyAuth } = require('../middleware/authMiddleware'); // Importar el middleware de autenticación

// --- Rutas GET ---
// GET /api/devices -> Devuelve una lista de todos los dispositivos.
// Esta ruta permite obtener todos los device_id únicos registrados en la base de datos.
router.get('/devices', apiKeyAuth, deviceController.listDevices);

// GET /api/devices/:deviceId/data -> Devuelve los últimos N registros de un dispositivo.
// Admite un query param ?limit=N para limitar la cantidad de registros devueltos.
router.get('/devices/:deviceId/data', apiKeyAuth, deviceController.getDeviceData);


// --- Ruta POST --- 
// POST /api/devices/:deviceId/command -> Envía un comando a un dispositivo.
// Esta ruta permite enviar comandos a un dispositivo específico a través de MQTT.
router.post('/devices/:deviceId/command', apiKeyAuth, deviceController.sendCommand);

// --- Exportar el router ---
// Esto permite que el router sea utilizado en la configuración principal de la API.
module.exports = router;