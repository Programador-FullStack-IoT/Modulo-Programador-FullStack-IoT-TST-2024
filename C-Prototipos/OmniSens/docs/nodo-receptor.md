# Nodo Receptor - OmniSens

Este microcontrolador (ESP32) cumple el rol de **Concentrador** entre la red LoRa y la red IP/MQTT. Se encarga de:

- Recibir los datos sensados enviados por LoRa.
- Publicarlos en la nube mediante el broker MQTT.
- Recibir comandos desde el dashboard (por MQTT) y reenviarlos vía LoRa al nodo actuador.

---

## 🔌 Funcionalidad principal

| Acción                           | Descripción                                                |
|----------------------------------|-------------------------------------------------------------|
| **LoRa RX**                      | Recibe JSON con datos sensados desde `microcontrolador-sensado`. |
| **MQTT TX**                      | Publica esos datos en diferentes `topics` del broker.      |
| **MQTT RX**                      | Escucha temas para recibir comandos del dashboard.         |
| **LoRa TX**                      | Reenvía comandos JSON por LoRa al nodo actuador.           |

---

## 🌐 MQTT

| Parámetro     | Valor                                |
|---------------|--------------------------------------|
| Broker        | `telecomunicaciones.ddns.net`        |
| Puerto MQTT   | `2480`                               |
| Usuario       | `telecomunicaciones`                 |
| Contraseña    | `cohorte*2024`                       |
| Topic RX      | `grupoAQI/ordenes`                   |
| Topic TX      | `grupoAQI/datos/temperatura`, etc.   |

---

## 🔁 Flujo general del nodo receptor

