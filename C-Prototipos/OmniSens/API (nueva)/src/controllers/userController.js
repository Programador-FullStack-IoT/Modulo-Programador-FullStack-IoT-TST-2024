// src/controllers/userController.js
const pool = require('../service/databaseService');

/**
 * @swagger
 * /api/users/count:
 * get:
 * summary: Obtiene la cantidad total de usuarios.
 * responses:
 * 200:
 * description: Cantidad total de usuarios.
 */
const getUsersCount = async (req, res) => {
  try {
    const [rows] = await pool.query('SELECT COUNT(*) as count FROM users');
    res.json(rows[0]);
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
};

/**
 * @swagger
 * /api/users:
 * get:
 * summary: Obtiene todos los usuarios.
 * responses:
 * 200:
 * description: Una lista de usuarios.
 */
const getAllUsers = async (req, res) => {
  try {
    const [rows] = await pool.query('SELECT username, api_key, created_at FROM users');
    res.json(rows);
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
};

/**
 * @swagger
 * /api/users/new:
 * get:
 * summary: Obtiene datos sobre la creación de nuevos usuarios a lo largo del tiempo.
 * responses:
 * 200:
 * description: Datos de crecimiento de usuarios.
 */
const getNewUsers = async (req, res) => {
  try {
    const [rows] = await pool.query('SELECT DATE(created_at) as date, COUNT(*) as count FROM users GROUP BY DATE(created_at) ORDER BY date');
    res.json(rows);
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
};

module.exports = {
  getUsersCount,
  getAllUsers,
  getNewUsers,
};