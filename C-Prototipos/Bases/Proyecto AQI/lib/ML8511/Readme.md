# Sensor ML8511

Este módulo implementa la clase `ML8511Sensor`, encargada de leer los valores de radiación ultravioleta (UV) utilizando el sensor **ML8511**.

Forma parte del sistema **OmniSens**, dentro del subsistema de sensores para monitoreo ambiental.

---

## 📦 Archivos incluidos

- `ML8511Sensor.cpp`: Implementación de la clase.
- `ML8511Sensor.h`: Definición de la clase y métodos públicos.

---

## 🔧 Dependencias

No requiere librerías externas específicas, solo la librería estándar de Arduino para lectura analógica.

---

## 📖 Descripción funcional

- Constructor: inicializa el objeto.
- `begin()`: Método vacío ya que el sensor ML8511 no requiere configuración especial.
- `leerUV()`: Lee el valor analógico del pin asignado al sensor (pin 33) y convierte esta señal en un valor proporcional a la radiación UV, escalado a un índice UV simplificado.

---

## 💡 ¿Para qué sirve el sensor ML8511?

Este sensor mide la intensidad de radiación ultravioleta ambiental, lo que es útil para monitorear la exposición UV, prevención de riesgos solares y estudios ambientales.
