// Middleware de autenticación por API Key para proteger rutas REST
const config = require('../config');

function apiKeyAuth(req, res, next) {
  const userApiKey = req.get('x-api-key')?.trim();
  const apiKey = process.env.API_KEY || config.api.apiKey;
  if (!userApiKey || userApiKey !== apiKey) {
    return res.status(401).json({ error: 'No autorizado. API Key inválida o no proporcionada.' });
  }
  next();
}

module.exports = { apiKeyAuth };
