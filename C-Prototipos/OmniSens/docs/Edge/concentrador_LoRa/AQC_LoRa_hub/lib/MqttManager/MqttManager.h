#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <functional>

/**
 * @brief Clase para gestionar la conexión y comunicación MQTT.
 * 
 * Encapsula PubSubClient, maneja reconexión automática, publicación y suscripción.
 */
class MqttManager {
public:
    /**
     * @brief Constructor.
     * @param server Dirección del broker MQTT.
     * @param port Puerto del broker MQTT.
     */
    MqttManager(const char* server, uint16_t port);

    /**
     * @brief Habilita/deshabilita mensajes de debug por Serial.
     */
    void setDebug(bool enable);

    /**
     * @brief Conecta al broker MQTT (con reconexión automática).
     */
    void connect();

    /**
     * @brief Procesa la comunicación MQTT (llamar periódicamente).
     */
    void loop();

    /**
     * @brief Publica un mensaje en un topic.
     */
    void publish(const char* topic, const char* payload);

    /**
     * @brief Se suscribe a un topic.
     */
    void subscribe(const char* topic);

    /**
     * @brief Registra un callback para órdenes recibidas por MQTT.
     */
    void onOrderReceived(std::function<void(char*, byte*, unsigned int)> callback);

private:
    const char* _server;
    uint16_t _port;
    WiFiClient _wifiClient;
    PubSubClient _mqttClient;
    bool _debug;
    std::function<void(char*, byte*, unsigned int)> _orderCallback;
    unsigned long _lastReconnectAttempt;
    void reconnect();
    static void _mqttCallback(char* topic, byte* payload, unsigned int length);
    static MqttManager* instance;
};

#endif