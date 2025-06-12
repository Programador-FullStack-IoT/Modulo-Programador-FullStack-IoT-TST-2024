const express = require('express');
const https = require('https');
const fs = require('fs');
const helmet = require('helmet');
const config = require('./src/config');
const deviceRoutes = require('./src/routes/deviceRoutes');
const userRoutes = require('./src/routes/userRoutes');
const grafanaRoutes = require('./src/routes/grafanaRoutes');
const errorHandler = require('./src/middleware/errorHandler');

// Iniciar servicios clave (rutas corregidas)
require('./src/service/databaseService');
require('./src/service/mqttService');
// -------------------------  

// Crear y configurar la aplicación Express
const app = express();
const port = process.env.PORT || 3000;
app.use(helmet());
app.use(express.json());

// Configurar las rutas de la API
app.use('/api', deviceRoutes);
app.use('/api', userRoutes);
app.use('/api/grafana', grafanaRoutes);
app.use('/api/users', userRoutes);

// Ruta de bienvenida para la raíz
app.get('/', (req, res) => {
  res.send('API OmniSens funcionando. Consulta los endpoints en /api.');
});

// Usar el middleware de manejo de errores (debe ser el último)
app.use(errorHandler);

app.listen(port, () => {
  console.log(`Servidor corriendo en http://localhost:${port}`);
});
// Opciones para el servidor HTTPS
// Nota: Asegúrate de tener una carpeta 'certs' con estos archivos o el servidor fallará.
// Para una prueba rápida sin HTTPS, puedes comentar todo el bloque https y usar "app.listen"
//const httpsOptions = {
//  key: fs.readFileSync('./certs/key.pem'),
//  cert: fs.readFileSync('./certs/cert.pem')
//};

// Crear y arrancar el servidor HTTPS
//https.createServer(httpsOptions, app).listen(config.api.port, () => {
//  console.log(`🚀 API Server SECURE (HTTPS) escuchando en https://localhost:${config.api.port}`);
//});