# Módulo LCDParallelDisplay

Este módulo implementa la clase `LCDParallelDisplay`, encargada de controlar una pantalla LCD 16x2 conectada en modo paralelo para mostrar información del sistema **OmniSens**.

Utiliza la librería `LiquidCrystal` para manejar la comunicación a través de pines digitales, mostrando menús y datos ambientales.

---

## 📦 Archivos incluidos

- `LCDParallelDisplay.cpp`: Implementación de la clase.
- `LCDParallelDisplay.h`: Definición de la clase y métodos públicos.
- Requiere: librería `LiquidCrystal`.

---

## 🔧 Dependencias

Este módulo depende de la librería `LiquidCrystal` para la comunicación con la pantalla LCD en modo paralelo.

---

## 📖 Descripción funcional

- Constructor: recibe los pines de conexión (RS, RW, D4, D5, D6, D7) usados para controlar el LCD.  
- `begin()`: Inicializa la pantalla LCD con 16 columnas y 2 filas y limpia la pantalla.  
- `displayMenu(int menu)`: Limpia la pantalla y muestra el título del menú seleccionado (0: Calidad del aire - AQI, 1: Temperatura y Humedad, 2: Luz y UV).  
- `updateMenu(int menu, float mq135, float temp, float humidity, float bmpTemp, float pressure, float light, float uv)`: Actualiza la segunda línea con datos ambientales según el menú:
    - Menú 0: valor MQ135 (calidad del aire).
    - Menú 1: temperatura y humedad.
    - Menú 2: luz y radiación UV.

---

## 💡 ¿Para qué sirve el módulo LCDParallelDisplay?

Este módulo permite mostrar datos ambientales en un display LCD clásico conectado con múltiples pines, ofreciendo una interfaz simple y directa para monitorear las mediciones del sistema sin necesidad de otros dispositivos.
