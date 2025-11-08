// Este módulo maneja la conexión a la base de datos y las operaciones de inserción de datos.
// Importamos el cliente de MySQL y la configuración de la base de datos
const mysql = require('mysql2/promise');
const config = require('../config');

// Configuración del pool de conexiones a la base de datos
// Permite manejar múltiples conexiones simultáneas de manera eficiente.
const pool = mysql.createPool({
  host: config.database.host, // Host de la base de datos
  port: config.database.port, // Puerto de la base de datos
  user: config.database.user, // Usuario de la base de datos
  password: config.database.password, // Contraseña de la base de datos
  database: config.database.database, // Nombre de la base de datos
  waitForConnections: true,
  connectionLimit: 10, // Máximo de conexiones simultáneas
  queueLimit: 0,
});

/**
 * Inserta un nuevo registro de medición en la base de datos.
 * @param {string} deviceId - El ID del dispositivo que envía los datos.
 * @param {object} data - El objeto JSON con los datos del sensor.
 * Se espera que el objeto data contenga las claves correspondientes a las columnas de la tabla.
 */
async function insertMeasurement(deviceId, data) {
  // Asumimos que la tabla se llama 'Measurements' y las columnas coinciden con las claves del JSON.
  // Es crucial que el JSON del dispositivo contenga las claves correspondientes a las columnas.
  // Ejemplo de JSON esperado: { "temperature": 25.5, "humidity": 60.1, "co2": 450.0 }
  
  const query = 'INSERT INTO Measurements (device_id, temperature, humidity, co2) VALUES (?, ?, ?, ?)';
  
  // Extraemos los valores del objeto de datos en el orden correcto.
  // Si una propiedad no existe, se insertará NULL (si la columna lo permite).
  const values = [
    deviceId,
    data.temperature,
    data.humidity,
    data.co2
  ];

  try {
    const [result] = await pool.query(query, values);
    console.log(`💾 Datos de ${deviceId} guardados en la DB. ID de registro: ${result.insertId}`);
    return result;
  } catch (error) {
    console.error(`❌ Error al guardar datos de ${deviceId} en la DB:`, error.message);
    // Podríamos añadir una lógica de reintento o notificación aquí si fuera necesario.
  }
}

/**
 * Obtiene una lista de todos los device_id únicos que han reportado mediciones.
 * Devuelve un array de objetos con los identificadores únicos.
 */
async function getAllDevices() {
  const query = 'SELECT DISTINCT device_id FROM Measurements ORDER BY device_id ASC';
  try {
    const [rows] = await pool.query(query);
    console.log('🔍 Consulta de dispositivos únicos ejecutada.');
    return rows;
  } catch (error) {
    console.error('❌ Error al consultar dispositivos en la DB:', error.message);
    throw error; // Propagar el error para que el controlador lo maneje
  }
}

/**
 * Obtiene los últimos registros de mediciones para un dispositivo específico.
 * @param {string} deviceId - El ID del dispositivo a consultar.
 * @param {number} limit - El número máximo de registros a devolver.
 * Realiza una consulta ordenada por timestamp descendente y limitada por el parámetro recibido.
 */
async function getDeviceDataById(deviceId, limit = 10) {
  // Asegurarse de que el límite sea un número para evitar inyecciones
  const numericLimit = parseInt(limit, 10);
  if (isNaN(numericLimit) || numericLimit <= 0) {
    limit = 10; // Valor por defecto si no es un número válido
  }

  const query = 'SELECT * FROM Measurements WHERE device_id = ? ORDER BY timestamp DESC LIMIT ?';
  const values = [deviceId, numericLimit];

  try {
    const [rows] = await pool.query(query, values);
    console.log(`🔍 Consulta de datos para ${deviceId} (límite: ${numericLimit}) ejecutada.`);
    return rows;
  } catch (error) {
    console.error(`❌ Error al consultar datos de ${deviceId} en la DB:`, error.message);
    throw error; // Propagar el error
  }
}

// Verificamos la conexión al iniciar el servicio
// Esto permite detectar problemas de conexión al arrancar la API.
pool.getConnection()
  .then(connection => {
    console.log('✅ Conectado exitosamente a la Base de Datos MariaDB');
    connection.release(); // Devolvemos la conexión al pool
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