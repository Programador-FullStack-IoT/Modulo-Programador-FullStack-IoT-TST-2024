const mysql = require('mysql2/promise');
const config = require('../config');

// Configuración del pool de conexiones a la base de datos 
// Permite manejar múltiples conexiones simultáneas de manera eficiente.
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

/**
 * Inserta un nuevo registro de medición en la base de datos.
 * @param {string} deviceId - El ID del dispositivo que envía los datos.
 * @param {object} data - El objeto JSON con los datos del sensor.
 * Se espera que el objeto data contenga: { temperatura1, humedad_relativa, gas_level }
 */
async function insertMeasurement(deviceId, data) {
  const query = `
    INSERT INTO sensor_data (device_id, temperatura1, humedad_relativa, gas_level)
    VALUES (?, ?, ?, ?)
  `;

  const values = [
    deviceId,
    data.temperatura1,
    data.humedad_relativa,
    data.gas_level
  ];

  try {
    const [result] = await pool.query(query, values);
    console.log(`💾 Datos de ${deviceId} guardados en la DB. ID de registro: ${result.insertId}`);
    return result;
  } catch (error) {
    console.error(`❌ Error al guardar datos de ${deviceId} en la DB:`, error.message);
  }
}

/**
 * Obtiene una lista de todos los device_id únicos que han reportado mediciones.
 * Devuelve un array de objetos con los identificadores únicos.
 */
async function getAllDevices() {
  const query = 'SELECT DISTINCT device_id FROM sensor_data ORDER BY device_id ASC';
  try {
    const [rows] = await pool.query(query);
    console.log('🔍 Consulta de dispositivos únicos ejecutada.');
    return rows;
  } catch (error) {
    console.error('❌ Error al consultar dispositivos en la DB:', error.message);
    throw error;
  }
}

/**
 * Obtiene los últimos registros de mediciones para un dispositivo específico.
 * @param {string} deviceId - El ID del dispositivo a consultar.
 * @param {number} limit - El número máximo de registros a devolver.
 * Realiza una consulta ordenada por timestamp descendente y limitada por el parámetro recibido.
 */
async function getDeviceDataById(deviceId, limit = 10) {
  const numericLimit = parseInt(limit, 10);
  const safeLimit = isNaN(numericLimit) || numericLimit <= 0 ? 10 : numericLimit;

  const query = `
    SELECT * FROM sensor_data
    WHERE device_id = ?
    ORDER BY timestamp DESC
    LIMIT ?
  `;
  const values = [deviceId, safeLimit];

  try {
    const [rows] = await pool.query(query, values);
    console.log(`🔍 Consulta de datos para ${deviceId} (límite: ${safeLimit}) ejecutada.`);
    return rows;
  } catch (error) {
    console.error(`❌ Error al consultar datos de ${deviceId} en la DB:`, error.message);
    throw error;
  }
}

// Verificamos la conexión al iniciar el servicio
pool.getConnection()
  .then(connection => {
    console.log('✅ Conectado exitosamente a la Base de Datos MariaDB');
    connection.release();
  })
  .catch(err => {
    console.error('❌ Error al conectar con la Base de Datos:', err.message);
  });

// Exportamos las funciones principales del servicio de base de datos
module.exports = {
  insertMeasurement,
  getAllDevices,
  getDeviceDataById,
};