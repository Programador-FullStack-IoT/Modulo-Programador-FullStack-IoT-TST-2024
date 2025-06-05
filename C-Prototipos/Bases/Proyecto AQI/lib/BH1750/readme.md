# Sensor BH1750

Este módulo implementa la clase `BH1750Sensor`, responsable de inicializar y leer datos del sensor de luz ambiente **BH1750**, utilizando la librería oficial `BH1750`.

Forma parte del sistema **OmniSens**, dentro del subsistema de sensores para monitoreo ambiental.

---

## 📦 Archivos incluidos

- `BH1750Sensor.cpp`: Implementación de la clase.  
- `BH1750Sensor.h`: Definición de la clase y métodos públicos.  
- Requiere: librerías `Wire` y `BH1750`.

---

## 🔧 Dependencias

Este módulo depende de las librerías oficiales `Wire` y `BH1750` para la comunicación I2C y manejo del sensor BH1750.

---

## 📖 Descripción funcional

- Constructor: inicializa el objeto sensor.  
- `begin()`: Configura e inicializa el sensor a través del bus I2C.  
- `leerLuz()`: Devuelve el nivel de luz ambiental en lux (lx).

---

## 💡 ¿Para qué sirve el sensor BH1750?

El sensor BH1750 permite medir con precisión la intensidad de luz ambiental en unidades de lux, lo que lo hace útil en sistemas de automatización, control de iluminación, monitoreo ambiental y aplicaciones IoT que requieren adaptación a condiciones de luz.
