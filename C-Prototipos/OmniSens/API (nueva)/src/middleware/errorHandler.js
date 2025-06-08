// Importar dependencias necesarias
// y configurar un middleware para manejar errores de forma centralizada.

/**
 * Middleware para capturar y manejar errores de forma centralizada.
 * @param {Error} err - El objeto de error.
 * @param {object} req - El objeto de petición de Express.
 * @param {object} res - El objeto de respuesta de Express.
 * @param {Function} next - La función para pasar al siguiente middleware.
 */
function errorHandler(err, req, res, next) {
  // Loggear el error para depuración (puedes integrarlo con un servicio de logging más avanzado)
  console.error(err.stack);

  // Enviar una respuesta de error genérica al cliente
  res.status(500).json({
    success: false,
    error: 'Ha ocurrido un error interno en el servidor.',
  });
}

module.exports = errorHandler;