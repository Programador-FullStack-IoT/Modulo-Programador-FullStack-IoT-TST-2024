// Importar las dependencias necesarias

const app = require('./app');
const config = require('./config');
const mqttService = require('./services/mqttService'); // <-- Importar

const port = config.api.port;

app.listen(port, () => {
  console.log(`Servidor de la API OmniSens escuchando en http://localhost:${port}`);
  
  // Iniciar la conexión con el broker MQTT
  mqttService.connect(); // <-- Añadido
});