# Sensor BH1750-GY302

Este módulo implementa la clase `BH1750_Sensor`, encargada de inicializar y leer datos del sensor de luz ambiente **BH1750-GY302** a través del protocolo I2C, usando la librería `Wire` para la comunicación.

El sensor mide la intensidad de luz en lux, lo que permite evaluar la iluminación ambiental para sistemas de monitoreo o control automático de luz.

Forma parte del sistema **OmniSens**, dentro del subsistema de sensores para monitoreo ambiental.

---

## 📦 Archivos incluidos

- `BH1750_Sensor.cpp`: Implementación de la clase.
- `BH1750_Sensor.h`: Definición de la clase y métodos públicos.

---

## 🔧 Dependencias

Este módulo depende de la librería oficial `Wire` para la comunicación I2C con el sensor BH1750.

---

## 📖 Descripción funcional

- `begin()`: Inicializa la comunicación I2C y configura el sensor para modo alta resolución.  
- `writeCommand(uint8_t cmd)`: Envía comandos al sensor por I2C (privado).  
- `readRawData()`: Lee los dos bytes de datos crudos de luz del sensor (privado).  
- `readLightLevel()`: Convierte los datos crudos a lux usando un factor de conversión (aproximado) y devuelve el valor en lux.

---

## 💡 ¿Para qué sirve el sensor BH1750-GY302?

Este sensor mide la iluminación ambiental en unidades de lux. Es útil para aplicaciones donde se requiere controlar la luz ambiental, como ajuste automático de iluminación, ahorro de energía o monitoreo ambiental en interiores o exteriores.
