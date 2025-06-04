const express = require('express');
/**
 * Importa el controlador de comandos.
 * El controlador 'commandController' gestiona la lógica relacionada con los comandos de la aplicación,
 * incluyendo la creación, actualización, eliminación y obtención de comandos.
 * 
 * @module commandController
 */
const commandController = require('../controllers/commandController');
const router = express.Router();

router.post('/devices/:deviceId/command', commandController.sendDeviceCommand);

module.exports = router;