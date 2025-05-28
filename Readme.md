## PROGRAMADOR FULLSTACK - GRUPO PLATA

![alt text](./E-%20Assets/logo.png)
# OmniSens – Sistema de Monitoreo y Control Ambiental Industrial

**OmniSens** es un sistema orientado al monitoreo y control ambiental en entornos industriales. Utiliza sensores para medir variables críticas como temperatura, humedad, presión y luminosidad, y transmite estos datos mediante tecnología **LoRa** hacia un servidor en la nube, permitiendo monitoreo y gestión en tiempo real.

---

## 🧑‍💻 Integrantes

| Nombre                        | GitHub                                 |
|------------------------------|----------------------------------------|
| Fernando Gimenez Coria       | [@FerCbr](https://github.com/FerCbr) |
| Paola Pantoja                | [@PaolaaPantoja](https://github.com/PaolaaPantoja) |
| Diego Ares                   | [@diegote7](https://github.com/diegote7) |
| Emma Vilma Gutierrez         | [@emygut](https://github.com/emygut) |
| Macarena Aylèn Carballo      | [@MacarenaAC](https://github.com/MacarenaAC) |
| Romina Vanesa Huk            | [@RoHu17](https://github.com/RoHu17) |
| Juan Diego Gonzalez Antoniazzi | [@JDGA1997](https://github.com/JDGA1997) |
| Leandro Roldan               | [@pleroldan](https://github.com/pleroldan) |
| Raul Jara                    | [@r-j28](https://github.com/r-j28) |
---

## Descripción General

El objetivo principal de OmniSens es implementar un sistema integral capaz de mantener condiciones óptimas de ambiente, garantizando la seguridad y calidad del aire. Para ello, el sistema incluye una serie de **controladores edge** que reciben los datos de los sensores y accionan dispositivos como extractores de aire, luces, alarmas sonoras y balizas luminosas.  
Las acciones de los controladores edge buscan regular parámetros ambientales: por ejemplo, ajustar la velocidad de extractores/ventiladores, la intensidad de luces, o activar alarmas cuando los valores de gases o variables ambientales sobrepasen los límites establecidos.

Los controladores edge se comunican mediante un enlace LoRa hacia otro controlador encargado de funcionar como **nodo concentrador**, formando una red de tipo estrella.  
Este **nodo concentrador** envia a su vez todos datos recolectados a un **broker MQTT** remoto para su posterior analisis y almacenamiento. Esta arquitectura otorga la posibilidad de implementar analisis de datos a nivel de edge, permitiendo reaccionar a variables criticas con baja latencia, sin dejar de lado el procesamiento en la nube, que se encarga de almacenar todos los sucesos y tambien enviar ordenes a los controladores Edge.   


Toda la información recolectada se analiza en un servidor remoto y es almacenada en una base de datos configurada para este fin, luego se procede a mostrar la informacion en un dashboard. Este diseño permite visualizar la informacion procesada a vez que puese enviar ordenes automaticas o comandos indicados por el usuario.

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

[![Image-25-may-2025-23-56-13.png](https://i.postimg.cc/59ZMqqC6/Chat-GPT-Image-25-may-2025-23-56-13.png)](https://postimg.cc/RqQ8vtGB)

## Objetivo del Proyecto

Desarrollar un sistema capaz de:
- Monitorear variables ambientales (temperatura, humedad, presión, luminosidad, gases) en tiempo real.
- Controlar dispositivos para mantener condiciones ambientales dentro de los parámetros de seguridad y calidad.
- Automatizar acciones correctivas (ventilación, iluminación, alarmas) ante condiciones anómalas.
- Registrar, almacenar y visualizar los datos históricos y en tiempo real para facilitar la toma de decisiones.

---

## Arquitectura del Sistema  

![TEXTO_ALTERNATIVO](./E-%20Assets/arquitectura_proyecto_ACTUALIZADA.png)

El sistema está compuesto por:
- **Sensores ambientales**: Medición de variables críticas.  
- **Actuadores**: Dispositivos tales como Extractores/ventiladores, relay o contactores para comando on/off, balizas y alarmas sonoras
- **Microcontroladores Edge con LoRa**: Son los encargados de tomar la informacion de los sensores y procesar variables criticas que requieran baja latencia, ademas envian la informacion adquirida por enlace LoRa hacia el concentrador.
- **Concentrador Lora**: Controlador encargado de recibir los datos provenientes de los controladores Edge. Recibe la información y la envia por MQTT al **broker remoto**, ademas se encarga de ditribuir los comandos provenientes del servidor remoto hacia los **Controladores Edge** para accionar los actuadores.  
- **Servidor remoto**: Este servidor cumple las funciones de plataforma en la nube, cuenta con varios contenedores docker conectados en una red virtual. La lista de contenedores es la siguiente:  
    - **Broker MQTT**: recepcion de los datos provenientes del Edge y envio de comandos hacia los controladores edge  
    - **API**: Recibe los datos del broker MQTT y los procesa para autenticar y autorizar a los dispositivos y usuarios.  
    - **Node-red**: recibe los datos de la API y los procesa para analizar y tomar decisiones. Ademas, envía las reacciones a los actuadores a través de MQTT y almacena los datos y resultados de analisis en una base de datos.  
    - **Base de datos**: Aqui se almacena la informacion de los sensores, asi como los analisis realizados por Node-RED para su posterior visualizacion.  
    - **Grafana**: Grafana se utiliza para crear el dashboard encargado de mostrar los datos historicos y en tiempo real del sistema.
- **Interfaz web o dashboard**: Monitoreo y control centralizado.  


### Flujo de Datos

1. Los sensores captan variables ambientales.
2. Los datos viajan por LoRa a un microcontrolador concentrador, que se encarga a la vez de enviarlos por MQTT a un servidor remoto
3. El servidor analiza y devuelve ordenes para que los **Controladores Edge** accionen los actuadores.
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

[![Chat-GPT-Image-28-may-2025-01-53-42.png](https://i.postimg.cc/hjsJ1wvq/Chat-GPT-Image-28-may-2025-01-53-42.png)](https://postimg.cc/tZ1CXBFr)

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
