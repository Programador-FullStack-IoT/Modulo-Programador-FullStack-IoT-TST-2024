
const express = require('express');
const router = express.Router();
// La ruta de importación ahora refleja la nueva estructura
const deviceController = require('../controllers/deviceController');

// --- Rutas GET ---
// GET /api/devices -> Devuelve una lista de todos los dispositivos.
router.get('/devices', deviceController.listDevices);

// GET /api/devices/:deviceId/data -> Devuelve los últimos N registros de un dispositivo.
// Admite un query param ?limit=N
router.get('/devices/:deviceId/data', deviceController.getDeviceData);


// --- Ruta POST --- 
// POST /api/devices/:deviceId/command -> Envía un comando a un dispositivo.
router.post('/devices/:deviceId/command', deviceController.sendCommand);

// --- Exportar el router ---
// Esto permite que el router sea utilizado en la configuración principal de la API.
module.exports = router;