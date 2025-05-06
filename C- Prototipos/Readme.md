# OmniSens Industrial Suite
Sistema de monitoreo ambiental en entornos industriales.

## Índice
1. [Descripción_General](#id1)  
  
2. [Arquitectura General](#id2)

3. [Estructura del Repositorio](#id3)

4. [Relación de Capas IoT con el Repositorio](#id4)

5. [Integración Industrial](#id5)

6. [Estado del Proyecto](#id6)

7. [Licencia](#id7)


## Descripción General<a name="id1"></a>

**OmniSens Industrial Suite** es una familia de productos diseñada para monitorear variables ambientales en entornos industriales, con foco en seguridad, eficiencia y adaptabilidad.

Incluye:

**OmniSens Hub:** Plataforma SaaS central de integración y visualización.

**OmniSens GasGuard:** Monitoreo de gases industriales.

**OmniSens TempWatch:** Monitoreo de temperatura en procesos.

**OmniSens LiquidMonitor:** Monitoreo de líquidos y parámetros asociados.

Todos los productos están integrados a través de OmniSens Hub para gestión remota, alertas, reportes y análisis avanzado de datos.

## Arquitectura General<a name="id2"></a>
```mermaid

flowchart TD
  A[Dispositivos IoT / Concentradores Industriales] --> B[Gateways Edge]
  B --> C[OmniSens Hub Backend]
  C --> D[OmniSens Hub Frontend]
  D --> E[Usuarios]
  C --> F[API de Integración Industrial (OPC UA, SCADA, etc.)]  
```    

## Estructura del Repositorio<a name="id3"></a>  

![Estructura_repositorio](../C-%20Prototipos/docs/assets/Estructura_repo.drawio.png)

## Relación de Capas IoT con el Repositorio<a name="id4"></a>

|Capa IoT	|Carpeta asociada	|Descripción|  
|-----------|-------------------|-----------|
|1. Dispositivos físicos	|/devices/	|Firmwares de sensores y actuadores|  
|2. Captura de datos	|/edge/	|Gateways y concentradores industriales|  
|3. Red de comunicación	|/configs/ y /edge/	|Configuraciones de protocolos de red|  
|4. Gestión de datos	|/hub/backend/	|Backend de recolección y almacenamiento|  
|5. Servicios de procesamiento	|/hub/backend/, /hub/frontend/	|Análisis y visualización|  
|6. Aplicación	|/hub/frontend/	|Interfaz de usuario y alertas|  
|7. Seguridad y gestión	|/hub/backend/auth/, /configs/security/	|Autenticación, autorización, políticas de gestión|  

## Integración Industrial<a name="id5"></a>
OmniSens está preparado para integrarse con entornos industriales existentes mediante:

OPC UA para sistemas SCADA/MES/DCS.

API RESTful para integración con otras aplicaciones.

Concentradores industriales que soportan PROFIBUS, Modbus TCP, EtherNet/IP, y otros protocolos industriales estándares.

Esto permite a OmniSens operar como una solución complementaria y expandible dentro del ecosistema industrial del cliente.

## Estado del Proyecto<a name="id6"></a>
En desarrollo inicial: definición de arquitectura y primeros módulos.

## Licencia<a name="id6"></a>
Este proyecto se encuentra bajo la Licencia MIT. Ver LICENSE para más detalles.