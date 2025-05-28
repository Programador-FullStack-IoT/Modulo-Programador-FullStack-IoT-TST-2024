Lista de Issues para organizar el desarrollo:  

🔹 Fase 1: Prototipo Funcional de Sensado y Control
    [ ] Implementar y probar la lectura de sensores (temperatura, humedad, presión, luminosidad, gases).

    [ ] Configurar la comunicación LoRa entre los microcontroladores Edge y el concentrador.

    [ ] Desarrollar los módulos de control de actuadores (extractores, luces, alarmas).

    [ ] Documentar la arquitectura del sistema en docs/architecture.md.

🔹 Fase 2: Backend y Base de Datos
    [ ] Definir la estructura de la base de datos y esquemas para el almacenamiento de datos de sensores y eventos del sistema.

    [ ] Desarrollar la API en el servidor para recibir datos de MQTT, autenticar y gestionar dispositivos/usuarios.

    [ ] Configurar el flujo en Node-RED para el procesamiento de datos, análisis y generación de respuestas automáticas.

    [ ] Integrar la API con Node-RED para asegurar que la transmisión de datos sea efectiva.

    [ ] Configurar Grafana para la visualización de datos en tiempo real e históricos.

    [ ] Validar la comunicación entre todos los módulos y documentar la integración en docs/integration.md.

🔹 Fase 3: Dashboard e Interfaz Web
    [ ] Desarrollar el frontend en hub/frontend/ para visualizar y gestionar los datos del sistema.

    [ ] Implementar la integración con Grafana para mostrar métricas en tiempo real.

    [ ] Añadir acciones de control remoto desde la interfaz web.

    [ ] Optimizar la interfaz de usuario para una experiencia intuitiva.

🔹 Fase 4: Integración y Pruebas
    [ ] Conectar todos los módulos y validar el flujo de datos entre sensores, LoRa, MQTT, API y Dashboard.

    [ ] Realizar pruebas en entorno simulado y ajustar parámetros de rendimiento y respuesta.

    [ ] Mejorar la latencia y eficiencia del procesamiento en los controladores Edge.

🔹 Fase 5: Documentación y Escalado
    [ ] Completar la documentación técnica y guías de instalación en docs/.

    [ ] Publicar tutoriales y guías de contribución para facilitar la integración de nuevos colaboradores.

    [ ] Planificar mejoras futuras, como la inclusión de más sensores y nuevos protocolos de comunicación.