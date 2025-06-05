# Sensor MQ135

Este módulo implementa la clase `MQ135Sensor`, diseñada para medir la calidad del aire utilizando el sensor de gases **MQ135**.

Forma parte del sistema **OmniSens**, dentro del subsistema de sensores de gases para monitoreo ambiental.

---

## 📦 Archivos incluidos

- `MQ135Sensor.cpp`: Implementación de la clase.
- `MQ135Sensor.h`: Definición de la clase y sus métodos.

---

## 🔧 Dependencias

Este módulo depende de la librería `MQ135`, que permite interactuar con el sensor desde el entorno Arduino.

---

## 📖 Descripción funcional

- **Constructor**: Recibe el pin analógico al que está conectado el sensor.
- **`begin()`**: Inicializa el arreglo de lecturas para comenzar con el filtrado por media móvil.
- **`readFilteredData()`**:  
  - Lee los valores de PPM (partes por millón) desde el sensor.
  - Aplica un filtro de media móvil de 10 muestras para suavizar las variaciones.
  - Devuelve el valor promedio como representación del índice de calidad del aire.

---

## 💡 ¿Para qué sirve el sensor MQ135?

El MQ135 es un sensor analógico diseñado para detectar gases como amoníaco, óxidos de nitrógeno, alcohol, benceno, humo y dióxido de carbono. Es ideal para sistemas de monitoreo ambiental que requieren conocer el nivel de contaminación o la calidad del aire.

---

## 📌 Observación

Este módulo implementa un sistema de **filtrado por media móvil**, lo que mejora la estabilidad de las lecturas y reduce el ruido de señal que puede aparecer por variaciones eléctricas o interferencias ambientales.
