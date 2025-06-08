
// Importamos las dependencias necesarias
// y configuramos la aplicación Express para manejar las rutas de la API.
const https = require('https'); // 1. Importar https
const fs = require('fs');       // 2. Importar fs para leer archivos
const express = require('express');
const helmet = require('helmet'); // 3. Importar helmet
const config = require('./config');
const deviceRoutes = require('./api/routes/deviceRoutes');
const errorHandler = require('./api/middleware/errorHandler'); // Importar el middleware



// Iniciar servicios clave
// Al importarlos, el código se ejecuta y los clientes se conectan
require('./services/mqttService');
require('./services/databaseService');

// Crear y configurar la aplicación Express
const app = express();
app.use(helmet()); // 4. Usar helmet para securizar las cabeceras HTTP
app.use(express.json()); // Middleware para parsear cuerpos JSON

// Configurar las rutas de la API
app.use('/api', deviceRoutes);

// 5. Usar el middleware de manejo de errores
// Debe ser el último middleware que se añade a la app.
app.use(errorHandler);

// 6. Opciones para el servidor HTTPS
const httpsOptions = {
  key: fs.readFileSync('./certs/key.pem'),
  cert: fs.readFileSync('./certs/cert.pem')
};

// 7. Crear y arrancar el servidor HTTPS
https.createServer(httpsOptions, app).listen(config.api.port, () => {
  console.log(`🚀 API Server SECURE (HTTPS) escuchando en https://localhost:${config.api.port}`);
});
