// Servicio para gestión de usuarios y métricas para Grafana
const mysql = require('mysql2/promise');
const config = require('../config');

const pool = mysql.createPool({
  host: config.database.host,
  port: config.database.port,
  user: config.database.user,
  password: config.database.password,
  database: config.database.database,
  waitForConnections: true,
  connectionLimit: 10,
  queueLimit: 0,
});

// Devuelve la cantidad total de usuarios
async function getUserCount() {
  const [rows] = await pool.query('SELECT COUNT(*) as count FROM users');
  return rows[0].count;
}

// Devuelve el listado de usuarios
async function getAllUsers() {
  const [rows] = await pool.query('SELECT id, username, created_at FROM users ORDER BY id ASC');
  return rows;
}

// Devuelve la cantidad de nuevos usuarios en un rango de fechas
async function getNewUsers(from, to) {
  const [rows] = await pool.query(
    'SELECT COUNT(*) as count FROM users WHERE created_at BETWEEN ? AND ?',
    [from, to]
  );
  return rows[0].count;
}

module.exports = {
  getUserCount,
  getAllUsers,
  getNewUsers,
};
