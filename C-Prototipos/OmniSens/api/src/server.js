// Importa la función para crear la app Express
const createApp = require('./app');
// Importa la configuración de la aplicación
const config = require('./config');
// Para HTTPS:
// const https = require('https');
// const fs = require('fs');
// const path = require('path');

async function startServer() {
    try {
        // Crea la instancia de la app
        const app = await createApp();

        // Servidor HTTP (para desarrollo o si hay un proxy inverso manejando SSL)
        app.listen(config.port, () => {
            console.log(`Servidor API escuchando en http://localhost:${config.port}`);
            console.log(`Entorno actual: ${config.env}`);
            console.log(`API Key para pruebas (si es la predeterminada): ${config.apiKey}`);
        });

        // Servidor HTTPS (hay que crear el certificado y configurar rutas correctas)

        const httpsOptions = {
            key: fs.readFileSync(path.join(__dirname, '../certs/private.key')), // Reemplazar con el archivo de clave
            cert: fs.readFileSync(path.join(__dirname, '../certs/certificate.crt')), // Reemplazar con el archivo de certificado
        };
        https.createServer(httpsOptions, app).listen(config.port, () => {
            console.log(`Servidor API escuchando de forma segura en https://localhost:${config.port}`);
            console.log(`Entorno actual: ${config.env}`);
        });
    

    } catch (error) {
        // Manejo de errores durante el inicio del servidor
        console.error("Error fatal durante el inicio del servidor:", error.message);
        process.exit(1);
    }
}

// Apagado elegante
function gracefulShutdown(signal) {
    console.log(`\nSe recibió ${signal}. Cerrando de forma elegante...`);
    // Obtiene el cliente MQTT y lo desconecta si está conectado
    const mqttClient = mqttService.getMqttClient();
    if (mqttClient && mqttClient.connected) {
        mqttClient.end(false, () => { // false = no forzar, permitir mensajes pendientes
            console.log('Cliente MQTT desconectado.');
        });
    }
    // Cierra el pool de la base de datos si existe
    const dbPool = dbService.getPool();
    if (dbPool) {
        dbPool.end(err => {
            if (err) console.error('Error al cerrar el pool de base de datos:', err.message);
            else console.log('Pool de base de datos cerrado.');
            process.exit(0);
        });
    } else {
        process.exit(0);
    }
    // Forzar apagado si el cierre elegante falla después del tiempo de espera
    setTimeout(() => {
        console.error('No se pudieron cerrar las conexiones a tiempo, apagando forzosamente');
        process.exit(1);
    }, 10000); // 10 segundos
}

// Escucha señales del sistema para apagado elegante
process.on('SIGINT', () => gracefulShutdown('SIGINT'));
process.on('SIGTERM', () => gracefulShutdown('SIGTERM'));

// Inicia el servidor
startServer();