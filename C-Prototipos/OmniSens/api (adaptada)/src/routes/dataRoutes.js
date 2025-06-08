// Rutas para la gestión de datos de dispositivos y sensores
// Incluye autenticación y delega la lógica al controlador de datos

const express = require('express');
const dataController = require('../controllers/dataController');
const router = express.Router();
const { apiKeyAuth } = require('../middleware/authMiddleware'); // Importa el middleware de autenticación

/**
 * @route   GET /api/devices
 * @desc    Obtiene una lista de todos los dispositivos
 * @access  Protegido por API Key
 */
router.get('/devices', apiKeyAuth, dataController.listDevices); // Aplica el middleware y llama al controlador

/**
 * @route   GET /api/devices/:deviceId/data
 * @desc    Obtiene los datos de un dispositivo específico. Soporta ?limit=N
 * @access  Protegido por API Key
 */
router.get('/devices/:deviceId/data', apiKeyAuth, dataController.getDeviceData); // Aplica el middleware y llama al controlador

module.exports = router; // Exporta el router para ser usado en la app principal