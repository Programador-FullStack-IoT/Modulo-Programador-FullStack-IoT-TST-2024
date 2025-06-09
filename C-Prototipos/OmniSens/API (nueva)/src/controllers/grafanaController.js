const mysql = require('mysql2/promise');
require('dotenv').config();

const dbConfig = {
  host: process.env.DB_HOST,
  port: process.env.DB_PORT,
  user: process.env.DB_USER,
  password: process.env.DB_PASSWORD,
  database: process.env.DB_DATABASE,
};

async function getConnection() {
  return await mysql.createConnection(dbConfig);
}

async function getUsersCount(req, res) {
  try {
    const connection = await getConnection();
    const [rows] = await connection.execute('SELECT COUNT(*) as count FROM users');
    await connection.end();
    res.json(rows[0]);
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
}

async function getUsers(req, res) {
  try {
    const connection = await getConnection();
    const [rows] = await connection.execute('SELECT name, api_key, created_at FROM users');
    await connection.end();
    res.json(rows);
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
}

async function getNewUsersByDay(req, res) {
  try {
    const connection = await getConnection();
    const [rows] = await connection.execute(`
      SELECT DATE(created_at) as date, COUNT(*) as count
      FROM users
      GROUP BY DATE(created_at)
      ORDER BY date
    `);
    await connection.end();
    res.json(rows);
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
}

module.exports = {
  getUsersCount,
  getUsers,
  getNewUsersByDay,
};