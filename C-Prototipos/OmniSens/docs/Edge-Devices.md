
# Dispositivos Edge en el Prototipo OmniSens

## Introducción

En el prototipo OmniSens, los dispositivos Edge son nodos inteligentes que recolectan y procesan datos de sensores localmente, en el "borde" de la red. Esta arquitectura permite un procesamiento eficiente y una comunicación optimizada con los servidores centrales o la nube.

## ¿Qué es un dispositivo Edge?

Un dispositivo Edge es un hardware que realiza procesamiento de datos cercano a la fuente de información, en lugar de enviar toda la información sin procesar a un servidor remoto. Esto reduce la latencia, mejora la respuesta en tiempo real y disminuye el consumo de ancho de banda.

## Funcionalidad de los dispositivos Edge en OmniSens

- Lectura de datos desde sensores conectados.
- Procesamiento local de señales, incluyendo filtrado y detección de eventos relevantes.
- Envío de datos procesados o alertas al servidor central utilizando protocolos MQTT o HTTP.
- Operación autónoma con bajo consumo energético.
- Gestión de comunicaciones confiables con el servidor central o la nube.

## Arquitectura técnica

- **Hardware:** Microcontroladores como ESP32 o STM32, con conectividad WiFi o Ethernet.
- **Software:** Firmware especializado para la lectura de sensores y procesamiento local, que implementa protocolos MQTT para la comunicación.
- **Comunicación:** Uso de MQTT para transmitir datos procesados y eventos al servidor central.
- **Seguridad:** Implementación de cifrado TLS/SSL para la transmisión segura de datos (si aplica).

## Beneficios de usar dispositivos Edge

- Reducción del tráfico de red al enviar solo datos relevantes.
- Mejor respuesta en tiempo real para aplicaciones críticas.
- Mayor robustez y autonomía de los nodos.
- Escalabilidad del sistema al distribuir la carga de procesamiento.

## Diagrama básico de flujo

```
[Sensores] --> [Dispositivo Edge (Procesamiento local)] --> [Servidor Central / Nube]
```

---

## Referencias

Para más detalles técnicos, consultar el código fuente y diagramas en la carpeta `src` y otros documentos relacionados en este repositorio.

---

*Este documento forma parte de la documentación técnica del prototipo OmniSens, desarrollado dentro del módulo Programador FullStack IoT TST 2024.*
