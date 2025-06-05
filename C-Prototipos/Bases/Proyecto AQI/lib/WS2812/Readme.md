# Controlador WS2812 (NeoPixel RGB)

Este módulo implementa la clase `WS2812Controller`, diseñada para controlar un LED RGB tipo **WS2812 (NeoPixel)** y mostrar visualmente el nivel de calidad del aire (**AQI**, *Air Quality Index*) mediante colores representativos.

Forma parte del sistema **OmniSens**, dentro del subsistema de **actuadores visuales para monitoreo ambiental**.

---

## 📦 Archivos incluidos

- `WS2812Controller.cpp`: Implementación de la clase.  
- `WS2812Controller.h`: Definición de la clase y sus métodos.

---

## 🔧 Dependencias

Este módulo depende de la librería `Adafruit NeoPixel`, que permite controlar LEDs tipo WS2812 desde el entorno Arduino.

---

## 📖 Descripción funcional

- **Constructor**: Inicializa el objeto con el pin digital al que está conectado el LED WS2812.
- **`begin()`**:  
  - Inicializa el objeto `Adafruit_NeoPixel` (con 1 LED).  
  - Apaga el LED al comenzar mediante `strip.show()`.
- **`updateLED(int aqi)`**:  
  - Evalúa el valor del AQI recibido como parámetro.  
  - Cambia el color del LED de acuerdo a la siguiente lógica:
    - 🟢 Verde: AQI ≤ 50 → Aire bueno  
    - 🟡 Amarillo: AQI ≤ 100 → Aire moderado  
    - 🔴 Rojo: AQI > 100 → Aire contaminado  
  - El color se actualiza inmediatamente con `strip.setPixelColor()` y `strip.show()`.

---

## 💡 ¿Para qué sirve?

El módulo permite una representación visual rápida e intuitiva del estado de la calidad del aire ambiental. Utilizando un LED WS2812, brinda una alerta visual sin necesidad de interpretar datos numéricos o gráficos.

Es un componente esencial dentro de OmniSens para facilitar la comprensión inmediata del estado ambiental, mejorando la interacción con el usuario.

---

## 📌 Observación

Este módulo está diseñado para funcionar con **un solo LED WS2812**, pero puede adaptarse fácilmente para controlar una tira completa modificando el parámetro del constructor de `Adafruit_NeoPixel`.

---

## ⚠️ Nota técnica

- El WS2812 opera a **5V**. Si se utiliza con placas que trabajan a **3.3V** (como el **ESP32**), puede requerirse un **conversor de nivel lógico** para la línea de datos.
- Se recomienda agregar:
  - ✅ Una **resistencia de 330Ω** en la línea de datos.  
  - ✅ Un **condensador de 1000µF** entre VCC y GND para estabilizar la alimentación.

---

## ✅ Estado

Módulo **probado y funcional**.  
Listo para su integración en el sistema **OmniSens** como componente de **salida visual ante condiciones ambientales**.
