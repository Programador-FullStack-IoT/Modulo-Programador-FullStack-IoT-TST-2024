![alt text](./E-%20Assets/logo.png)
# OmniSens – Sistema de Monitoreo y Control Ambiental Industrial

**OmniSens** es un sistema orientado al monitoreo y control ambiental en entornos industriales. Utiliza sensores para medir variables críticas como temperatura, humedad, presión y luminosidad, y transmite estos datos mediante tecnología **LoRa** hacia un servidor en la nube, permitiendo monitoreo y gestión en tiempo real.

---

## Descripción General

El objetivo principal de OmniSens es implementar un sistema integral capaz de mantener condiciones óptimas de ambiente, garantizando la seguridad y calidad del aire. Para ello, el sistema incluye un **controlador** que acciona dispositivos como extractores de aire, luces, alarmas sonoras y balizas luminosas. Las acciones del controlador buscan regular parámetros ambientales: por ejemplo, ajustar la velocidad de ventiladores, la intensidad de luces, o activar alarmas cuando los valores de gases o variables ambientales sobrepasen los límites establecidos.

La comunicación se realiza a través de un **enlace LoRa punto a punto** entre microcontroladores:
- Un microcontrolador capta los datos de los sensores.
- Otro recibe la información.
- Un tercero gestiona los actuadores.

Toda la información recolectada se almacena y visualiza mediante un servidor y base de datos configurados para este fin, lo que permite monitoreo en tiempo real y la toma de decisiones automáticas para mantener el ambiente controlado.

El proyecto busca aprovechar prototipos y desarrollos previos, adaptando y escalando el enfoque hacia la gestión ambiental industrial. Además, se promueve el trabajo colaborativo y la comunicación de avances con los profesores para su evaluación.

---

## Índice

1.  [Descripción General](#descripción-general)
2.  [Objetivo del Proyecto](#objetivo-del-proyecto)
3.  [Arquitectura del Sistema](#arquitectura-del-sistema)
4.  [Componentes Principales](#componentes-principales)
5.  [Estructura del Repositorio](#estructura-del-repositorio)
6.  [Integración y Escalabilidad](#integración-y-escalabilidad)
7.  [Documentación](#documentación)
8.  [Hoja de Ruta (Roadmap)](#hoja-de-ruta-roadmap)
9.  [Estado del Proyecto](#estado-del-proyecto)
10. [Licencia](#licencia)
11. [Instalación](#instalación)
12. [Desarrollo y Contribución](#desarrollo-y-contribución)
13. [Próximos Pasos](#próximos-pasos)

---

## Objetivo del Proyecto

Desarrollar un sistema capaz de:
- Monitorear variables ambientales (temperatura, humedad, presión, luminosidad, gases) en tiempo real.
- Controlar dispositivos para mantener condiciones ambientales dentro de los parámetros de seguridad y calidad.
- Automatizar acciones correctivas (ventilación, iluminación, alarmas) ante condiciones anómalas.
- Registrar, almacenar y visualizar los datos históricos y en tiempo real para facilitar la toma de decisiones.

---

## Arquitectura del Sistema

El sistema está compuesto por:
- **Sensores ambientales**: Medición de variables críticas.
- **Microcontroladores con LoRa**: Envío punto a punto de datos entre módulos.
- **Controlador de actuadores**: Recibe la información y acciona extractores, luces, alarmas y balizas.
- **Servidor y base de datos**: Recepción, almacenamiento y visualización de información.
- **Interfaz web o dashboard**: Monitoreo y control centralizado.

### Flujo de Datos

1. Los sensores captan variables ambientales.
2. Los datos viajan por LoRa a un microcontrolador receptor.
3. El microcontrolador de control acciona los actuadores según las condiciones.
4. El servidor almacena y muestra los datos en tiempo real, permitiendo monitoreo remoto y acciones automáticas.

---

## Componentes Principales

- **Sensores**: Temperatura, humedad, presión, luminosidad, gases.
- **Red LoRa**: Comunicación eficiente y de largo alcance entre módulos.
- **Actuadores**: Extractores de aire, luces, alarmas, balizas.
- **Servidor y Dashboard**: Visualización de datos y control de dispositivos.

---

## Estructura del Repositorio

```
OmniSens/
├── README.md
├── LICENSE
├── docs/
│   ├── architecture.md
│   ├── device-specs.md
│   ├── integration.md
│   └── roadmap.md
├── hub/
│   ├── backend/
│   └── frontend/
├── devices/
│   ├── sensores/
│   └── actuadores/
├── edge/
│   ├── microcontrolador-sensado/
│   ├── microcontrolador-receptor/
│   └── microcontrolador-actuador/
├── configs/
├── deployments/
└── tests/
```

---

## Integración y Escalabilidad

El proyecto está pensado para aprovechar y escalar a partir de prototipos y códigos de años anteriores, integrando nuevas funcionalidades y adaptándose a distintas necesidades industriales. Se fomenta la colaboración y la mejora continua.

---

## Documentación

La documentación técnica se encuentra en la carpeta `docs/`, donde podrás encontrar:
- Detalles de la arquitectura y el flujo de datos.
- Especificaciones de dispositivos y protocolos utilizados.
- Estrategias de integración y pruebas.
- Roadmap de desarrollo.

---

## Hoja de Ruta (Roadmap)

El desarrollo de OmniSens se organiza en las siguientes fases y entregables principales:

### Fase 1: Prototipo Funcional de Sensado y Control
- [ ] Diseño y armado de red básica de sensores (temperatura, humedad, presión, luminosidad, gases).
- [ ] Configuración de comunicación LoRa entre microcontroladores (sensado, receptor y actuador).
- [ ] Implementación de control básico de actuadores (extractores, luces, alarmas).
- [ ] Documentación de arquitectura y conexiones en `docs/architecture.md`.

### Fase 2: Backend y Base de Datos
- [ ] Desarrollo del backend (`hub/backend/`) para recibir y almacenar datos.
- [ ] Configuración y pruebas de la base de datos.
- [ ] Documentación de la API y endpoints en `docs/integration.md`.

### Fase 3: Dashboard e Interfaz Web
- [ ] Desarrollo del frontend (`hub/frontend/`) para visualización y control.
- [ ] Implementación de monitoreo en tiempo real y registro histórico.
- [ ] Integración de acciones de control remoto desde el dashboard.

### Fase 4: Integración y Pruebas
- [ ] Integración de módulos (`edge`, `devices`, `hub`).
- [ ] Pruebas de funcionamiento en entorno simulado o real.
- [ ] Ajustes y mejoras tras feedback de pruebas.

### Fase 5: Documentación y Escalado
- [ ] Completar la documentación técnica en `docs/`.
- [ ] Publicar guías de instalación y uso para todos los módulos.
- [ ] Planificar siguientes iteraciones (nuevos sensores, protocolos, expansiones).


---

## Estado del Proyecto

- En desarrollo inicial: definición de la arquitectura y primeros módulos funcionales.

---

## Licencia

Este proyecto se encuentra bajo la Licencia MIT. Consulta el archivo LICENSE para más detalles.

---

## Instalación

Próximamente se publicarán las guías de instalación y configuración de los distintos módulos (sensado, comunicación LoRa, servidor y dashboard).

---

## Desarrollo y Contribución

Las contribuciones están abiertas. Si deseas participar:
1. Haz un fork y clona el repositorio.
2. Crea una rama para tu aporte.
3. Realiza los cambios y verifica que todo funcione correctamente.
4. Envía un Pull Request con una descripción clara del aporte.

---

## Próximos Pasos

- Desarrollar y probar el prototipo funcional de la red de sensores y el controlador de actuadores.
- Implementar el servidor y la visualización de datos.
- Integrar y validar el sistema en un entorno industrial simulado.
- Comunicar avances y resultados para la evaluación docente.

---
