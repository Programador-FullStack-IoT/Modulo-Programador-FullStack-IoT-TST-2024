# 📂 Requisitos y Fases del Proyecto

> **Carpeta:** `A-Requisitos`  
> **Descripción:** Esta sección detalla los requisitos y el plan de desarrollo de la plataforma **OmniSens**, organizado en fases incrementales.

---

## 🚦 Fases del Proyecto

### 🔵 Fase 1: Prototipo Funcional de Sensado y Control

> **Objetivo:** Construir y validar el hardware principal, asegurando la recolección de datos y el control de actuadores.

- [ ] **Lectura de sensores:** temperatura, humedad, presión, luminosidad, gases.
- [ ] **Comunicación LoRa:** entre microcontroladores Edge y el concentrador.
- [ ] **Control de actuadores:** extractores, luces, alarmas.
- [ ] **Documentación:** arquitectura del sistema en [`docs/architecture.md`](../docs/architecture.md).

---

### 🟢 Fase 2: Backend y Base de Datos

> **Objetivo:** Desarrollar la infraestructura de servidor para recibir, almacenar, procesar y visualizar datos.

- [ ] **Definir base de datos:** esquemas para sensores y eventos.
- [ ] **API del servidor:** recepción de MQTT, autenticación, gestión de dispositivos/usuarios.
- [ ] **Node-RED:** procesamiento, análisis y respuestas automáticas.
- [ ] **Integración API + Node-RED:** transmisión de datos efectiva.
- [ ] **Grafana:** visualización de datos en tiempo real e históricos.
- [ ] **Validación e integración:** documentar en [`docs/integration.md`](../docs/integration.md).

---

### 🟡 Fase 3: Dashboard e Interfaz Web

> **Objetivo:** Crear una interfaz amigable para la interacción y control remoto.

- [ ] **Frontend:** desarrollo en `hub/frontend/` para visualizar y gestionar datos.
- [ ] **Integración con Grafana:** métricas en tiempo real.
- [ ] **Acciones remotas:** control desde la web.
- [ ] **Optimización UX:** experiencia intuitiva.

---

### 🟠 Fase 4: Integración y Pruebas

> **Objetivo:** Unificar componentes y realizar pruebas exhaustivas.

- [ ] **Conexión de módulos:** validar flujo de datos entre sensores, LoRa, MQTT, API y Dashboard.
- [ ] **Pruebas simuladas:** ajustar parámetros de rendimiento y respuesta.
- [ ] **Mejoras en Edge:** latencia y eficiencia.

---

### 🟣 Fase 5: Documentación y Escalado

> **Objetivo:** Finalizar documentación, facilitar colaboración y planificar el crecimiento futuro.

- [ ] **Documentación técnica:** guías de instalación en `docs/`.
- [ ] **Tutoriales y contribución:** facilitar integración de nuevos colaboradores.
- [ ] **Planificación de mejoras:** nuevos sensores y protocolos.

---