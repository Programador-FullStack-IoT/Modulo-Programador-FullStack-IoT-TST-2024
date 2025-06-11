# 📚 Índice de Documentación - Proyecto OmniSens

| Documento                     | Descripción                                                                 |
|-------------------------------|-----------------------------------------------------------------------------|
| [`arquitectura.md`](./arquitectura.md)         | Visión general del sistema, componentes y flujo de datos.                  |
| [`integracion.md`](./integracion.md)           | Explica cómo se conectan todos los nodos y servicios.                     |
| [`nodo-sensado.md`](./nodo-sensado.md)         | Detalles técnicos del nodo de captura de datos (ESP32 + sensores).       |
| [`nodo-receptor.md`](./nodo-receptor.md)       | Función de Concentrador utilizando LoRa–MQTT, publica datos al broker.                    |
| [`nodo-actuador.md`](./nodo-actuador.md)       | Recibe comandos y activa relés, PWM o LED baliza.                        |
| [`backend.md`](./backend.md)                   | Procesamiento de datos, publicación/lectura MQTT, conexión con base.     |
| [`frontend.md`](./frontend.md)                 | Interfaz gráfica de monitoreo y control, uso de Node-RED UI / Grafana.  |
