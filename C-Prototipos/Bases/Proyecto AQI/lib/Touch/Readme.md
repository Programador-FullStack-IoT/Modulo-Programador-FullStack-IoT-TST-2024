# Controlador Táctil (Touch)

Este módulo implementa la clase `TouchController`, que permite detectar entradas táctiles mediante pines capacitivos, simulando botones físicos como "Siguiente" o "Anterior".

Forma parte del sistema **OmniSens**, dentro del subsistema de interacción con el usuario.

---

## 📦 Archivos incluidos

- `TouchController.cpp`: Implementación de la clase.
- `TouchController.h`: Definición de la clase y métodos públicos.

---

## 🔧 Dependencias

- Requiere soporte para `touchRead()`, disponible en placas como **ESP32**.
- No utiliza librerías externas adicionales.

---

## 📖 Descripción funcional

- **Constructor**: Recibe los pines táctiles a utilizar.
- **`begin()`**: No requiere inicialización especial para los sensores táctiles.
- **`checkTouch()`**:
  - Lee el valor táctil de los pines configurados.
  - Si el valor es menor a un umbral (30), se interpreta como un toque.
  - Devuelve:
    - `1` → Toque en el pin de avance ("Next").
    - `2` → Toque en el pin de retroceso ("Previous").
    - `0` → Sin toque detectado.

---

## 🧠 ¿Para qué sirve?

Los sensores táctiles permiten crear interfaces sin botones físicos, mejorando la durabilidad y el diseño. En este caso, se usan para navegar entre menús o vistas del sistema OmniSens en pantallas LCD.

---

## ⚠️ Nota técnica

- El valor umbral de 30 puede necesitar ajustes dependiendo del entorno físico (humedad, interferencias, etc.).
- Ideal para microcontroladores como **ESP32**, que tienen entradas táctiles nativas.
