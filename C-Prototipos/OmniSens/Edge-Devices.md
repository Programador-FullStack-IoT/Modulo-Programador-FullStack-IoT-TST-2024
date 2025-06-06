
# Dispositivos Edge en el Proyecto OmniSens

## Introducción

En el proyecto **OmniSens**, los dispositivos Edge son componentes fundamentales que permiten el procesamiento local de datos provenientes de sensores, antes de ser enviados a la nube o a servidores centrales. Esta arquitectura de computación en el borde (edge computing) mejora la eficiencia, reduce la latencia y optimiza el uso del ancho de banda.

## Rol de los Dispositivos Edge

Los dispositivos Edge en OmniSens cumplen las siguientes funciones:

- **Adquisición de datos**: Recopilan información de diversos sensores (por ejemplo, temperatura, humedad, presión).
- **Procesamiento local**: Realizan análisis preliminares, como filtrado de ruido y detección de eventos relevantes.
- **Comunicación eficiente**: Transmiten únicamente los datos procesados o eventos significativos al servidor central, utilizando protocolos como MQTT o HTTP.
- **Autonomía operativa**: Pueden funcionar de manera independiente en caso de pérdida de conexión con la nube, almacenando datos localmente hasta restablecer la comunicación.

## Hardware Utilizado

Los dispositivos Edge en OmniSens están basados en microcontroladores y microprocesadores de bajo consumo y alta eficiencia. Algunos ejemplos incluyen:

- **ESP32**: Microcontrolador con conectividad Wi-Fi y Bluetooth, ideal para aplicaciones IoT.
- **Raspberry Pi**: Microcomputadora de bajo costo y tamaño reducido, adecuada para tareas de procesamiento más complejas.

Estos dispositivos pueden estar equipados con módulos de comunicación adicionales, como LoRa o Zigbee, dependiendo de los requisitos específicos del entorno de implementación.

## Software y Firmware

El software que opera en los dispositivos Edge está diseñado para ser ligero y eficiente. Sus características principales incluyen:

- **Lectura de sensores**: Interfaces para adquirir datos de diversos tipos de sensores.
- **Procesamiento de datos**: Algoritmos para análisis en tiempo real, detección de anomalías y toma de decisiones locales.
- **Gestión de comunicaciones**: Implementación de protocolos como MQTT para la transmisión de datos al servidor central.
- **Actualizaciones remotas**: Capacidad para recibir actualizaciones de firmware de forma remota, facilitando el mantenimiento y la mejora continua.

El desarrollo del firmware se realiza utilizando lenguajes como C/C++ o Python, y puede incluir bibliotecas específicas para la gestión de sensores y comunicaciones.

## Seguridad y Mantenimiento

La seguridad es un aspecto crucial en la arquitectura Edge de OmniSens. Se implementan medidas como:

- **Autenticación y autorización**: Verificación de identidad de los dispositivos antes de permitir el intercambio de datos.
- **Cifrado de datos**: Uso de protocolos seguros (TLS/SSL) para proteger la información transmitida.
- **Monitoreo y alertas**: Sistemas para detectar comportamientos anómalos y generar alertas en tiempo real.

Además, se establece un plan de mantenimiento preventivo que incluye la supervisión del estado de los dispositivos, actualizaciones periódicas y reemplazo de componentes en caso necesario.

## Beneficios de la Arquitectura Edge en OmniSens

- **Reducción de latencia**: Al procesar datos localmente, se obtienen respuestas más rápidas ante eventos críticos.
- **Eficiencia en el uso del ancho de banda**: Se transmiten únicamente los datos relevantes, disminuyendo la carga en la red.
- **Escalabilidad**: La arquitectura permite agregar nuevos dispositivos Edge sin afectar significativamente el rendimiento del sistema.
- **Resiliencia**: Los dispositivos pueden operar de manera autónoma en caso de fallos en la conexión con la nube.

## Diagrama de Flujo Simplificado

```
[Sensores] --> [Dispositivo Edge (Procesamiento local)] --> [Servidor Central / Nube]
```

## Conclusión

Los dispositivos Edge son componentes esenciales en la arquitectura del proyecto OmniSens, permitiendo una gestión eficiente y segura de los datos provenientes de sensores en entornos IoT. Su implementación adecuada contribuye significativamente al rendimiento y la fiabilidad del sistema en su conjunto.

---

*Este documento forma parte de la documentación técnica del prototipo OmniSens, desarrollado dentro del módulo Programador FullStack IoT TST 2024.*
