const express = require('express');
const config = require('./config');

// Importar rutas
const dataRoutes = require('./routes/dataRoutes');
const commandRoutes = require('./routes/commandRoutes'); // <-- Importar rutas de comandos

const app = express();

// Middlewares
app.use(express.json());

// Usar rutas
app.use('/api', dataRoutes);
app.use('/api', commandRoutes); // <-- Usar rutas de comandos

module.exports = app;