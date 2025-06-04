const mqtt = require('mqtt');
const config = require('../config');
const dbService = require('./dbService');

let client;

// Función para conectar al broker MQTT
function connectMqtt() {
    const options = {
        username: config.mqtt.username, // Usuario MQTT
        password: config.mqtt.password, // Contraseña MQTT
        clientId: `omnisens_api_${Math.random().toString(16).slice(2, 10)}`, // ID único de cliente
        reconnectPeriod: 5000, // Intentar reconectar cada 5 segundos
        connectTimeout: 10 * 1000, // Tiempo de espera de conexión: 10 segundos
    };

    client = mqtt.connect(config.mqtt.brokerUrl, options);

    // Evento al conectar exitosamente
    client.on('connect', () => {
        console.log(`Connected to MQTT broker: ${config.mqtt.brokerUrl}`);
        // Suscribirse al tópico de datos
        client.subscribe(config.mqtt.dataTopicPattern, (err) => {
            if (err) console.error(`Failed to subscribe to data topic (${config.mqtt.dataTopicPattern}):`, err.message);
            else console.log(`Subscribed to data topic: ${config.mqtt.dataTopicPattern}`);
        });
        // Suscribirse al tópico de estado
        client.subscribe(config.mqtt.statusTopicPattern, (err) => {
            if (err) console.error(`Failed to subscribe to status topic (${config.mqtt.statusTopicPattern}):`, err.message);
            else console.log(`Subscribed to status topic: ${config.mqtt.statusTopicPattern}`);
        });
    });

    // Evento al recibir un mensaje
    client.on('message', async (topic, message) => {
        console.log(`MQTT Message | Topic: ${topic} | Payload: ${message.toString()}`);
        try {
            const payload = JSON.parse(message.toString());
            const topicParts = topic.split('/');
            
            // Validar estructura del tópico
            if (topicParts.length < 4 || topicParts[0] !== 'omnisens' || topicParts[1] !== 'devices') {
                console.warn(`Ignoring message on unexpected topic structure: ${topic}`);
                return;
            }
            const deviceId = topicParts[2];
            const messageType = topicParts[3];

            // Normalizar el timestamp
            payload.timestamp = payload.timestamp 
                ? new Date(payload.timestamp).toISOString().slice(0, 19).replace('T', ' ')
                : new Date().toISOString().slice(0, 19).replace('T', ' ');

            // Procesar mensaje según el tipo
            if (messageType === 'data') {
                await dbService.storeSensorData(deviceId, payload); // Guardar datos de sensores
            } else if (messageType === 'status') {
                // Guardar estado de actuadores si existen los campos
                if (payload.rele1_status !== undefined || payload.rele2_status !== undefined || payload.pwm_value !== undefined) {
                        await dbService.storeActuatorStatus(deviceId, payload);
                } else {
                        console.log(`Received generic status for ${deviceId}. Define specific fields or store in a generic log.`);
                }
            } else {
                console.warn(`Unhandled message type '${messageType}' on topic ${topic}`);
            }
        } catch (error) {
            console.error('Error processing MQTT message:', error.message, 'Topic:', topic, 'Payload:', message.toString());
        }
    });

    // Manejo de errores y eventos de conexión
    client.on('error', (error) => console.error('MQTT client error:', error.message));
    client.on('close', () => console.log('MQTT client disconnected.'));
    client.on('offline', () => console.log('MQTT client is offline.'));
    client.on('reconnect', () => console.log('MQTT client reconnecting...'));
}

// Función para publicar un comando a un dispositivo
function publishCommand(deviceId, command) {
    if (!client || !client.connected) {
        console.error('MQTT client not connected. Cannot publish command.');
        throw new Error('MQTT client not connected.');
    }
    const topic = `${config.mqtt.commandTopicPrefix}${deviceId}/command`;
    const message = JSON.stringify(command);
    
    client.publish(topic, message, { qos: 1, retain: false }, (err) => {
        if (err) {
            console.error(`Failed to publish command to ${topic}:`, err.message);
            throw err;
        } else {
            console.log(`Command published to ${topic}: ${message}`);
            // Registrar el comando en la base de datos
            dbService.logCommand(deviceId, command).catch(logError => {
                    console.error('Error logging command to DB:', logError.message);
            });
        }
    });
}

// Obtener la instancia del cliente MQTT
function getMqttClient() {
        return client;
}

module.exports = {
    connectMqtt,
    publishCommand,
    getMqttClient,
};