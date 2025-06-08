// Archivo principal para iniciar el servidor de la API
// Arranca Express y la conexión con el broker MQTT

// Importa la app Express configurada
const app = require('./app');
// Importa la configuración global (puerto, claves, etc.)
const config = require('./config');
// Importa el servicio MQTT para iniciar la conexión al broker
const mqttService = require('./services/mqttService');

const port = config.api.port; // Puerto en el que se ejecutará la API

// Inicia el servidor HTTP y la conexión MQTT
app.listen(port, () => {
  console.log(`Servidor de la API OmniSens escuchando en http://localhost:${port}`);
  
  // Iniciar la conexión con el broker MQTT
  mqttService.connect();
});