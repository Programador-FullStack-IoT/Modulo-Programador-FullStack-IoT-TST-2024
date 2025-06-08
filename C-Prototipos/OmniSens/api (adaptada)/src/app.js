// Configuración y arranque de la aplicación Express
// Importa rutas y aplica middlewares globales

const express = require('express');
const config = require('./config');

// Importar rutas de datos y comandos
const dataRoutes = require('./routes/dataRoutes');
const commandRoutes = require('./routes/commandRoutes');

const app = express(); // Crea la instancia principal de la app Express

// Middlewares globales
app.use(express.json()); // Permite recibir y parsear JSON en las peticiones

// Usar rutas para la API REST
app.use('/api', dataRoutes);    // Rutas para datos de dispositivos
app.use('/api', commandRoutes); // Rutas para comandos a dispositivos

// Exporta la instancia de la app para ser utilizada por el servidor principal
module.exports = app;