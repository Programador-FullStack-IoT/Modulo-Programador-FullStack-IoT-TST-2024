
const config = require('../config');

/**
 * Middleware para verificar la API Key en las cabeceras de la petición.
 * @param {object} req - Objeto de solicitud de Express.
 * @param {object} res - Objeto de respuesta de Express.
 * @param {function} next - Función para pasar al siguiente middleware.
 */
const apiKeyAuth = (req, res, next) => {
  const userApiKey = req.get('x-api-key'); // El cliente debe enviar la clave en esta cabecera

  if (!userApiKey || userApiKey !== config.api.apiKey) {
    return res.status(401).json({ error: 'No autorizado. API Key inválida o no proporcionada.' });
  }

  // Si la clave es válida, continuamos con la siguiente función
  next();
};

module.exports = {
  apiKeyAuth,
};