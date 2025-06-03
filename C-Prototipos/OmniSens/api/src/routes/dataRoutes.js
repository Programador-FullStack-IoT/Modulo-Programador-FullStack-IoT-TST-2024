const express = require('express');
/**
 * Controlador que gestiona las operaciones relacionadas con los datos.
 * Importa las funciones necesarias para manejar las solicitudes HTTP
 * asociadas a los datos en la aplicación.
 * 
 * @module dataController
 */
const dataController = require('../controllers/dataController');
const router = express.Router();

router.get('/devices', dataController.listDevices);
router.get('/devices/:deviceId/data', dataController.getDeviceData);
router.get('/devices/:deviceId/data/latest', dataController.getLatestDeviceData);

module.exports = router;