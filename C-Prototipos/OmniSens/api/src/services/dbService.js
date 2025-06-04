const mysql = require('mysql2/promise');
const config = require('../config');

let pool;

// Inicializa la conexión al pool de la base de datos MySQL
async function initDb() {
    try {
        pool = mysql.createPool({
            host: config.db.host,
            port: config.db.port,
            user: config.db.user,
            password: config.db.password,
            database: config.db.name,
            waitForConnections: true,
            connectionLimit: 10,
            queueLimit: 0,
            timezone: '+00:00', // Almacenar y recuperar fechas en UTC
        });
        console.log('Database pool created successfully.');
        const connection = await pool.getConnection();
        console.log(`Successfully connected to the database: ${config.db.name} on ${config.db.host}`);
        connection.release();
    } catch (error) {
        console.error('Failed to create database pool or connect:', error.message);
        process.exit(1);
    }
}

// Asegura que el dispositivo exista en la tabla devices, si no lo crea
async function ensureDeviceExists(deviceId, defaultName = null) {
    const nameToUse = defaultName || `Device ${deviceId}`;
    await pool.execute(
        'INSERT INTO devices (device_id, name) VALUES (?, ?) ON DUPLICATE KEY UPDATE name=IFNULL(devices.name, VALUES(name))',
        [deviceId, nameToUse]
    );
}

// Almacena datos de sensores en la base de datos
async function storeSensorData(deviceId, data) {
    if (!pool) throw new Error('Database pool not initialized.');
    const {
        timestamp, temperatura1, temperatura2, uv, humedad_relativa,
        presion_atm, luz, gas_level, codigo_alarma,
    } = data;

    await ensureDeviceExists(deviceId);

    const sql = `
        INSERT INTO sensor_data
        (device_id, timestamp, temperatura1, temperatura2, uv, humedad_relativa, presion_atm, luz, gas_level, codigo_alarma)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
    `;
    const params = [
        deviceId, timestamp, temperatura1, temperatura2, uv, humedad_relativa,
        presion_atm, luz, gas_level, codigo_alarma,
    ];
    try {
        const [result] = await pool.execute(sql, params);
        console.log(`Sensor data for device ${deviceId} stored with ID: ${result.insertId}`);
        return result.insertId;
    } catch (error) {
        console.error(`Error storing sensor data for device ${deviceId}:`, error.message);
        throw error;
    }
}

// Almacena el estado de los actuadores en la base de datos
async function storeActuatorStatus(deviceId, data) {
    if (!pool) throw new Error('Database pool not initialized.');
    const { timestamp, rele1_status, rele2_status, pwm_value } = data;

    await ensureDeviceExists(deviceId);

    const sql = `
            INSERT INTO actuator_status
            (device_id, timestamp, rele1_status, rele2_status, pwm_value)
            VALUES (?, ?, ?, ?, ?)
    `;
    const params = [deviceId, timestamp, rele1_status, rele2_status, pwm_value];
    try {
            const [result] = await pool.execute(sql, params);
            console.log(`Actuator status for device ${deviceId} stored with ID: ${result.insertId}`);
            return result.insertId;
    } catch (error) {
            console.error(`Error storing actuator status for device ${deviceId}:`, error.message);
            throw error;
    }
}

// Obtiene datos de sensores de un dispositivo, con filtros de paginación y fechas
async function getSensorData(deviceId, { limit = 100, offset = 0, startDate, endDate }) {
    if (!pool) throw new Error('Database pool not initialized.');
    let sql = 'SELECT * FROM sensor_data WHERE device_id = ?';
    const params = [deviceId];

    if (startDate) {
        sql += ' AND timestamp >= ?';
        params.push(startDate);
    }
    if (endDate) {
        sql += ' AND timestamp <= ?';
        params.push(endDate);
    }

    sql += ' ORDER BY timestamp DESC LIMIT ? OFFSET ?';
    params.push(parseInt(limit, 10), parseInt(offset, 10));

    try {
        const [rows] = await pool.query(sql, params);
        return rows;
    } catch (error) {
        console.error(`Error fetching sensor data for device ${deviceId}:`, error.message);
        throw error;
    }
}

// Obtiene el dato más reciente de sensores para un dispositivo
async function getLatestSensorData(deviceId) {
    if (!pool) throw new Error('Database pool not initialized.');
    const sql = 'SELECT * FROM sensor_data WHERE device_id = ? ORDER BY timestamp DESC LIMIT 1';
    try {
        const [rows] = await pool.query(sql, [deviceId]);
        return rows.length > 0 ? rows[0] : null;
    } catch (error) {
        console.error(`Error fetching latest sensor data for device ${deviceId}:`, error.message);
        throw error;
    }
}

// Obtiene la lista de dispositivos registrados
async function getDevices() {
    if (!pool) throw new Error('Database pool not initialized.');
    const sql = 'SELECT device_id, name, type, location, created_at, updated_at FROM devices';
    try {
        const [rows] = await pool.query(sql);
        return rows;
    } catch (error) {
        console.error('Error fetching devices:', error.message);
        throw error;
    }
}

// Registra un comando enviado a un dispositivo y su respuesta (si existe)
async function logCommand(deviceId, command, response = null, responseReceivedAt = null) {
    if (!pool) throw new Error('Database pool not initialized.');
    const sql = `
            INSERT INTO commands_log (device_id, command, response, response_received_at)
            VALUES (?, ?, ?, ?)
    `;
    const params = [deviceId, JSON.stringify(command), response ? JSON.stringify(response) : null, responseReceivedAt];
    try {
            const [result] = await pool.execute(sql, params);
            console.log(`Command for device ${deviceId} logged with ID: ${result.insertId}`);
            return result.insertId;
    } catch (error) {
            console.error(`Error logging command for device ${deviceId}:`, error.message);
            throw error;
    }
}

// Valida si una API key es válida y está activa
async function isValidApiKey(apiKey) {
    if (!pool) throw new Error('Database pool not initialized.');
    const sql = 'SELECT api_key FROM api_users WHERE api_key = ? AND is_active = TRUE';
    try {
            const [rows] = await pool.query(sql, [apiKey]);
            return rows.length > 0;
    } catch (error) {
            console.error('Error validating API key:', error.message);
            return false; // Falla de forma segura
    }
}

// Exporta las funciones para ser usadas en otros módulos
module.exports = {
    initDb,
    storeSensorData,
    storeActuatorStatus,
    getSensorData,
    getLatestSensorData,
    getDevices,
    logCommand,
    isValidApiKey,
    getPool: () => pool,
};