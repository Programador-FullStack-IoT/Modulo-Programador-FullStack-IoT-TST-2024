const config = require('../config');

/**
 * Middleware para verificar la API Key en las cabeceras de la petición.
 * @param {object} req - Objeto de solicitud de Express.
 * @param {object} res - Objeto de respuesta de Express.
 * @param {function} next - Función para pasar al siguiente middleware.
 */
// Middleware de autenticación por API Key
// Verifica que la clave enviada en la cabecera sea válida antes de permitir el acceso a la ruta
const apiKeyAuth = (req, res, next) => {
  const userApiKey = req.get('x-api-key'); // Obtiene la clave enviada por el cliente en la cabecera

  // Si la clave no existe o no coincide con la configurada, rechaza la petición
  if (!userApiKey || userApiKey !== config.api.apiKey) {
    return res.status(401).json({ error: 'No autorizado. API Key inválida o no proporcionada.' });
  }

  // Si la clave es válida, continúa con la siguiente función/middleware
  next();
};

// Exporta el middleware para ser usado en las rutas protegidas
module.exports = {
  apiKeyAuth,
};