const express = require('express');
const bodyParser = require('body-parser');
const cors = require('cors');
const { apiKeyAuth } = require('./middleware/authMiddleware');
const dataRoutes = require('./routes/dataRoutes');
const commandRoutes = require('./routes/commandRoutes');
const dbService = require('./services/dbService');
const mqttService = require('./services/mqttService');
const config = require('./config');

/**
 * Crea y configura una instancia de la aplicación Express.
 *
 * Inicializa la base de datos y la conexión MQTT, configura middlewares para CORS, 
 * parsing de JSON y URL-encoded, define rutas de salud y rutas protegidas por API key,
 * y gestiona errores 404 y errores no controlados.
 *
 * @async
 * @function
 * @returns {Promise<import('express').Express>} Una promesa que resuelve con la aplicación Express configurada.
 */
async function createApp() {
    const app = express();

    await dbService.initDb();
    mqttService.connectMqtt();

    app.use(cors({ origin: '*' })); // Configurar apropiadamente para producción
    app.use(bodyParser.json());
    app.use(bodyParser.urlencoded({ extended: true }));

    app.get('/health', (req, res) => {
        const mqttClient = mqttService.getMqttClient();
        res.status(200).json({ 
                status: 'UP', 
                timestamp: new Date().toISOString(),
                mqttConnected: mqttClient ? mqttClient.connected : false,
                dbPoolInitialized: !!dbService.getPool(),
                environment: config.env,
        });
    });

    // Rutas de API seguras
    app.use('/api', apiKeyAuth); 
    app.use('/api', dataRoutes);
    app.use('/api', commandRoutes);
    
    app.use((req, res) => {
        res.status(404).json({ error: 'Not Found', message: `Endpoint ${req.method} ${req.originalUrl} not found.` });
    });

    app.use((err, req, res, next) => {
        console.error("Unhandled error:", err.stack || err.message || err);
        res.status(err.status || 500).json({ 
                error: 'Internal Server Error', 
                message: err.message || 'An unexpected error occurred.'
        });
    });

    return app;
}

module.exports = createApp;