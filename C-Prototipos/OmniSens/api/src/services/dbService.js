// Servicio para la gestión de la base de datos (MariaDB/MySQL)
// Incluye funciones para CRUD de dispositivos, tipos de sensores y mediciones
// Descripción: Este servicio maneja la conexión a la base de datos y las operaciones CRUD básicas.
// Este archivo es parte del servicio de base de datos de la API OmniSens.

const mysql = require('mysql2/promise'); // Importa el cliente MySQL compatible con promesas
const config = require('../config');      // Importa la configuración global

// Pool de conexiones para mejorar el rendimiento y reutilizar conexiones
const pool = mysql.createPool(config.db);

/**
 * Obtiene el ID de un dispositivo desde la DB. Si no existe, lo crea.
 * @param {string} deviceEui - El EUI único del dispositivo.
 * @param {object} connection - La conexión de la base de datos a usar.
 * @returns {Promise<number>} - El ID del dispositivo en la base de datos.
 */
const getOrCreateDevice = async (deviceEui, connection) => {
  let [rows] = await connection.execute('SELECT id FROM devices WHERE device_eui = ?', [deviceEui]);
  if (rows.length > 0) {
    return rows[0].id;
  } else {
    const [result] = await connection.execute(
      'INSERT INTO devices (device_eui, name) VALUES (?, ?)',
      [deviceEui, `Dispositivo ${deviceEui}`]
    );
    return result.insertId;
  }
};

/**
 * Obtiene el ID de un tipo de sensor desde la DB. Si no existe, lo crea.
 * @param {string} sensorName - El nombre del sensor (ej. "temperature").
 * @param {object} connection - La conexión de la base de datos a usar.
 * @returns {Promise<number>} - El ID del tipo de sensor en la base de datos.
 */
const getOrCreateSensorType = async (sensorName, connection) => {
  let [rows] = await connection.execute('SELECT id FROM sensor_types WHERE name = ?', [sensorName]);
  if (rows.length > 0) {
    return rows[0].id;
  } else {
    // La unidad puede ser nula por defecto, la agregaremos más adelante si es necesario.
    const [result] = await connection.execute('INSERT INTO sensor_types (name) VALUES (?)', [sensorName]);
    return result.insertId;
  }
};

/**
 * Guarda los datos de los sensores en la base de datos dentro de una transacción.
 * @param {string} deviceEui - El EUI del dispositivo que envía los datos.
 * @param {object} data - El objeto JSON con los datos de los sensores.
 */
const saveSensorData = async (deviceEui, data) => {
  console.log(`Intentando guardar datos para el dispositivo ${deviceEui}`);
  const connection = await pool.getConnection();

  try {
    await connection.beginTransaction();

    const deviceId = await getOrCreateDevice(deviceEui, connection);

    for (const sensorName in data) {
      if (Object.hasOwnProperty.call(data, sensorName)) {
        const value = data[sensorName];
        const sensorTypeId = await getOrCreateSensorType(sensorName, connection);

        const sql = 'INSERT INTO measurements (device_id, sensor_type_id, value) VALUES (?, ?, ?)';
        await connection.execute(sql, [deviceId, sensorTypeId, value]);
        console.log(`  -> Medición guardada: ${sensorName}: ${value}`);
      }
    }

    await connection.commit();
    console.log(`Datos para ${deviceEui} guardados exitosamente.`);
  } catch (error) {
    await connection.rollback();
    console.error(`Error al guardar datos para ${deviceEui}. Transacción revertida.`, error);
  } finally {
    connection.release();
  }
};

/**
 * Obtiene todos los dispositivos registrados en la base de datos.
 * @returns {Promise<Array>} - Una lista de todos los dispositivos.
 */
const getAllDevices = async () => {
  const [rows] = await pool.execute(
    'SELECT device_eui, name, location, created_at FROM devices ORDER BY created_at DESC'
  );
  return rows;
};

/**
 * Obtiene las últimas mediciones de un dispositivo específico.
 * @param {string} deviceEui - El EUI del dispositivo a consultar.
 * @param {number} limit - La cantidad máxima de registros a devolver.
 * @returns {Promise<Array>} - Una lista de mediciones.
 */
const getMeasurementsByDevice = async (deviceEui, limit = 20) => {
  const sql = `
    SELECT
      m.value,
      m.timestamp,
      st.name AS sensor_type,
      st.unit
    FROM measurements AS m
    JOIN devices AS d ON m.device_id = d.id
    JOIN sensor_types AS st ON m.sensor_type_id = st.id
    WHERE d.device_eui = ?
    ORDER BY m.timestamp DESC
    LIMIT ?;
  `;
  const [rows] = await pool.execute(sql, [deviceEui, limit]);
  return rows;
};

// Exporta las funciones principales del servicio de base de datos
module.exports = {
  pool,
  saveSensorData,
  getAllDevices,
  getMeasurementsByDevice,
};