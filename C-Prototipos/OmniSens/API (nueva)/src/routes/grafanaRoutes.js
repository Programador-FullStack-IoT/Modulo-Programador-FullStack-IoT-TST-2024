const express = require('express');
const router = express.Router();
const grafanaController = require('../controllers/grafanaController');

router.get('/users/count', grafanaController.getUsersCount);
router.get('/users', grafanaController.getUsers);
router.get('/users/new-by-day', grafanaController.getNewUsersByDay);

module.exports = router;