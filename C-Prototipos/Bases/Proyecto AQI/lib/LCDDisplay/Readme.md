# Módulo LCDDisplay

Este módulo implementa la clase `LCDDisplay`, encargada de controlar una pantalla LCD I2C 16x2 para mostrar información relevante del sistema **OmniSens**.

Se utiliza la librería `LiquidCrystal_I2C` para la comunicación con la pantalla, permitiendo mostrar menús y datos ambientales en tiempo real.

---

## 📦 Archivos incluidos

- `LCDDisplay.cpp`: Implementación de la clase.
- `LCDDisplay.h`: Definición de la clase y métodos públicos.
- Requiere: librería `LiquidCrystal_I2C`.

---

## 🔧 Dependencias

Este módulo depende de la librería `LiquidCrystal_I2C` para la comunicación con la pantalla LCD vía protocolo I2C.

---

## 📖 Descripción funcional

- `begin()`: Inicializa la pantalla LCD con tamaño 16 columnas por 2 filas y activa la retroiluminación.  
- `displayMenu(int menu)`: Limpia la pantalla y muestra el título del menú seleccionado (0: Calidad del aire - AQI, 1: Temperatura y Humedad, 2: Luz y UV).  
- `updateMenu(int menu, float mq135, float temp, float humidity, float bmpTemp, float pressure, float light, float uv)`: Actualiza la segunda línea de la pantalla con datos específicos según el menú seleccionado:
    - Menú 0: muestra el valor del sensor MQ135 (calidad del aire).
    - Menú 1: muestra temperatura y humedad.
    - Menú 2: muestra niveles de luz y radiación UV.

---

## 💡 ¿Para qué sirve el módulo LCDDisplay?

Este módulo permite al sistema mostrar de forma clara y rápida diferentes datos ambientales medidos por los sensores, facilitando el monitoreo local sin necesidad de un PC o dispositivo externo. Es fundamental para la interacción directa con el usuario en el entorno industrial.
