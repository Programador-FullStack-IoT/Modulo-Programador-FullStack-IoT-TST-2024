const express = require('express');
const commandController = require('../controllers/commandController');
const router = express.Router();
const { apiKeyAuth } = require('../middleware/authMiddleware'); // <-- Importar middleware

/**
 * @route   POST /api/devices/:deviceId/command
 * @desc    Envía un comando a un dispositivo
 * @access  Public (por ahora)
 */
router.post('/devices/:deviceId/command', apiKeyAuth, commandController.sendCommand); // <-- Middleware aplicado

module.exports = router;