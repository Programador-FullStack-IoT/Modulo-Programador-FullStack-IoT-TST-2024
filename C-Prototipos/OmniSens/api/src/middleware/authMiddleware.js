const config = require('../config');
const dbService = require('../services/dbService');

/**
 * Middleware para autenticar solicitudes usando una clave API.
 * 
 * Extrae la clave API del encabezado 'x-api-key' de la solicitud y verifica su validez.
 * Si la clave API no está presente o no es válida, responde con un error 401 o 403 respectivamente.
 * Si la clave es válida, permite que la solicitud continúe al siguiente middleware.
 * 
 * @async
 * @function
 * @param {import('express').Request} req - Objeto de solicitud de Express.
 * @param {import('express').Response} res - Objeto de respuesta de Express.
 * @param {import('express').NextFunction} next - Función para pasar al siguiente middleware.
 */
async function apiKeyAuth(req, res, next) {
    const apiKey = req.headers['x-api-key'];
    if (!apiKey) {
        return res.status(401).json({ error: 'Unauthorized: API key is missing' });
    }

    // Para mayor seguridad, valida la clave API contra la base de datos
    const isValid = await dbService.isValidApiKey(apiKey);
    // const isValid = apiKey === config.apiKey; // Chequeo más simple contra .env

    if (!isValid) {
        return res.status(403).json({ error: 'Forbidden: Invalid API key' });
    }
    next();
}

module.exports = { apiKeyAuth };