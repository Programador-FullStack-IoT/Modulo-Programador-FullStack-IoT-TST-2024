# Sensor AHT25

Este módulo implementa la clase `AHT25Sensor`, encargada de inicializar y leer datos del sensor de temperatura y humedad **AHT25**, utilizando la librería oficial `Adafruit_AHTX0`.

Forma parte del sistema **OmniSens**, dentro del subsistema de sensores para monitoreo ambiental.

---

## 📦 Archivos incluidos

- `AHT25Sensor.cpp`: Implementación de la clase.  
- `AHT25Sensor.h`: Definición de la clase y métodos públicos.  
- Requiere: librería `Adafruit_AHTX0`.

---

## 🔧 Dependencias

Este módulo depende de la librería oficial **Adafruit AHTX0** para la comunicación con el sensor AHT25.

---

## 📖 Descripción funcional

- Constructor: inicializa el objeto sensor.  
- `begin()`: Inicializa el sensor AHT25 mediante la librería oficial.  
- `leerTemperatura()`: Devuelve la temperatura ambiental en grados Celsius.  
- `leerHumedad()`: Devuelve la humedad relativa en porcentaje.

---

## 💡 ¿Para qué sirve el sensor AHT25?

El sensor AHT25 mide temperatura y humedad ambiental con alta precisión, siendo ideal para aplicaciones de monitoreo ambiental, control climático, y sistemas IoT para el seguimiento de condiciones atmosféricas.
