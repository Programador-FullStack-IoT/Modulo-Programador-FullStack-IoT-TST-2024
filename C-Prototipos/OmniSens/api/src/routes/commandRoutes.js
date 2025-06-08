// Rutas para el envío de comandos a dispositivos
// Aplica el middleware de autenticación y delega la lógica al controlador correspondiente

const express = require('express');
const commandController = require('../controllers/commandController');
const router = express.Router();
const { apiKeyAuth } = require('../middleware/authMiddleware'); // Importa el middleware de autenticación

/**
 * @route   POST /api/devices/:deviceId/command
 * @desc    Envía un comando a un dispositivo
 * @access  Protegido por API Key
 */
router.post('/devices/:deviceId/command', apiKeyAuth, commandController.sendCommand); // Aplica el middleware y llama al controlador

module.exports = router; // Exporta el router para ser usado en la app principal