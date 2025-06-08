const express = require('express');
const dataController = require('../controllers/dataController');
const router = express.Router();
const { apiKeyAuth } = require('../middleware/authMiddleware'); // <-- Importar middleware

/**
 * @route   GET /api/devices
 * @desc    Obtiene una lista de todos los dispositivos
 * @access  Public (por ahora)
 */
router.get('/devices', apiKeyAuth, dataController.listDevices); // <-- Middleware aplicado

/**
 * @route   GET /api/devices/:deviceId/data
 * @desc    Obtiene los datos de un dispositivo específico. Soporta ?limit=N
 * @access  Public (por ahora)
 */
router.get('/devices/:deviceId/data', apiKeyAuth, dataController.getDeviceData); // <-- Middleware aplicado

module.exports = router;