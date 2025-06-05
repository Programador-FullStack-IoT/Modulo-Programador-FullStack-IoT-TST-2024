# Sensor BMP280

Este módulo implementa la clase `BMP280Sensor`, encargada de inicializar y leer datos del sensor de temperatura y presión atmosférica **BMP280**, utilizando la librería oficial `Adafruit_BMP280`.

El sensor mide la temperatura en grados Celsius y la presión atmosférica en hectopascales (hPa), datos útiles para monitoreo ambiental y aplicaciones meteorológicas.

Forma parte del sistema **OmniSens**, dentro del subsistema de sensores para monitoreo ambiental.

---

## 📦 Archivos incluidos

- `BMP280Sensor.cpp`: Implementación de la clase.
- `BMP280Sensor.h`: Definición de la clase y métodos públicos.
- Requiere: librería `Adafruit_BMP280`.

---

## 🔧 Dependencias

Este módulo depende de la librería oficial `Adafruit_BMP280` para la comunicación con el sensor BMP280.

---

## 📖 Descripción funcional

- `begin()`: Inicializa el sensor en la dirección I2C 0x76 (o 0x77).  
- `readData(float &temperature, float &pressure)`: Lee la temperatura (°C) y presión (hPa) del sensor, y realiza un filtrado básico para descartar datos fuera de rango. Devuelve `true` si la lectura es válida, `false` si no.

---

## 💡 ¿Para qué sirve el sensor BMP280?

Este sensor es utilizado para medir la temperatura ambiente y la presión atmosférica. Es muy útil en sistemas de monitoreo climático, estaciones meteorológicas, y para determinar condiciones ambientales que afectan otros procesos industriales o de control ambiental.
