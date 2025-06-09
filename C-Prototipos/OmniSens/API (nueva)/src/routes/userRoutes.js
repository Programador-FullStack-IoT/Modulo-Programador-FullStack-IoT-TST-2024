const express = require('express');
const router = express.Router();
const userController = require('../controllers/userController');
const { apiKeyAuth } = require('../middleware/authMiddleware');

// GET /api/users/count - Cantidad total de usuarios
router.get('/count', userController.getUsersCount);

// GET /api/users - Listado de usuarios
router.get('/', userController.getAllUsers);

// GET /api/users/new?from=YYYY-MM-DD&to=YYYY-MM-DD - Nuevos usuarios en rango
router.get('/new', userController.getNewUsers);

module.exports = router;
