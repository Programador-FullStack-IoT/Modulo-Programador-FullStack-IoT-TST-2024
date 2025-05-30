#ifndef ENLACELORACONCENTRADOR_H
#define ENLACELORACONCENTRADOR_H

#include <Arduino.h>
#include <RH_RF95.h>
#include <functional>
#include <map>

/**
 * @brief Clase para gestionar la comunicación LoRa en el concentrador.
 * 
 * Permite recibir mensajes de múltiples nodos, validar tokens, enviar ACK y retransmitir mensajes.
 */
class EnlaceLoraConcentrador {
public:
    /**
     * @brief Constructor.
     * @param csPin Pin CS del módulo LoRa.
     * @param rstPin Pin RST del módulo LoRa.
     * @param intPin Pin INT del módulo LoRa.
     */
    EnlaceLoraConcentrador(uint8_t csPin, uint8_t rstPin, uint8_t intPin);

    /**
     * @brief Inicializa el módulo LoRa.
     * @param freqMHz Frecuencia en MHz (por defecto 433.0).
     * @return true si inicializa correctamente.
     */
    bool begin(float freqMHz = 433.0);

    /**
     * @brief Habilita/deshabilita mensajes de debug por Serial.
     */
    void setDebug(bool enable);

    /**
     * @brief Procesa la recepción de mensajes LoRa (llamar periódicamente).
     */
    void loop();

    /**
     * @brief Envía un mensaje JSON por LoRa.
     */
    void sendJson(const String& json);

    /**
     * @brief Envía un ACK a un nodo específico.
     */
    void sendAck(const String& nodeId);

    /**
     * @brief Registra un callback para mensajes JSON recibidos.
     */
    void onJsonReceived(std::function<void(const String&)> callback);

    /**
     * @brief Establece el mapa de tokens válidos (node_id -> token).
     */
    void setTokenMap(const std::map<String, String>& tokens);

private:
    uint8_t _csPin, _rstPin, _intPin;
    bool _debug;
    RH_RF95* _rf95;
    std::function<void(const String&)> _jsonCallback;
    std::map<String, String> _tokenMap;
    void processReceived();
    bool validateToken(const String& nodeId, const String& token);
};

#endif