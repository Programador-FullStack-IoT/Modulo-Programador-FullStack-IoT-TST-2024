# Puente LoRa-MQTT para Control Remoto de LED

Este proyecto IoT integra dos dispositivos para controlar remotamente un LED usando LoRa y MQTT:

- **ESP32 (Emisor):**  
  Ubicado en una zona remota, el ESP32 mide la luminosidad con un sensor BH1750 y controla un LED secundario. Se comunica vía LoRa, recibiendo comandos ("LED_ON" o "LED_OFF") y enviando lecturas de luz. *(No se conecta a WiFi)*

- **ESP8266 (Puente MQTT):**  
  Instalado en el interior, este dispositivo (configurado con VSCode + PlatformIO) se conecta a WiFi y a un broker MQTT. Recibe comandos MQTT ("ON", "OFF", "AUTO") y los reenvía al ESP32 por LoRa. En modo forzado, reenvía el último comando cada 10 segundos para mejorar la confiabilidad, y vuelve a modo automático tras 1 minuto de inactividad.

---

## Características

- **Control Remoto:** Permite forzar el estado del LED del ESP32 mediante comandos MQTT.
- **Modos de Operación:**  
  - *Modo Forzado:* Se activa con "ON" o "OFF" y se reenvía el comando periódicamente vía LoRa.  
  - *Modo Automático:* Se activa con "AUTO" o tras 1 minuto sin comandos, permitiendo que el ESP32 controle el LED según la luz.
- **Reenvío Periódico:** Asegura la recepción del comando en entornos con posibles pérdidas de paquetes.

---

## Requisitos de Hardware

- ESP32 (emisor)
- ESP8266 (por ejemplo, NodeMCU, receptor y puente MQTT)
- Módulos LoRa (433 MHz)
- Sensor BH1750 (para medir luminosidad)
- Cables y conexiones necesarios

---

## Dependencias y Entorno

El proyecto utiliza las siguientes librerías:
- [LoRa](https://github.com/sandeepmistry/arduino-LoRa)
- [PubSubClient](https://pubsubclient.knolleary.net/)
- [ESP8266WiFi](https://github.com/esp8266/Arduino)
- [ArduinoJson](https://arduinojson.org/)
- [BH1750](https://github.com/claws/BH1750)
- [SPI](https://www.arduino.cc/en/Reference/SPI)
- [Wire](https://www.arduino.cc/en/Reference/Wire)

Se recomienda utilizar VSCode con PlatformIO. Un ejemplo de `platformio.ini` para el ESP8266 podría ser:

```ini
[env:esp8266]
platform = espressif8266
board = nodemcuv2
framework = arduino
monitor_speed = 9600
build_flags = -DLORA_SS=15 -DLORA_RST=0 -DLORA_DIO0=5 -DLED_REMOTO_PIN=2
lib_deps =
  pubsubclient
  ArduinoJson
  ESP8266WiFi
  sandeepmistry/arduino-LoRa
```  
---   

## Configuración e Instalación

Clonar el repositorio: Descarga o clona este proyecto y ábrelo con VSCode.  

- Configuración del ESP8266:

    - Actualiza las variables ssid y password con los datos de tu red WiFi.

    - Ajusta los parámetros MQTT (mqtt_server, mqtt_port, mqtt_user, mqtt_password).

    - Verifica que los pines definidos (LoRa y LED) coincidan con tu hardware.

- Compilación y Subida: Utiliza el comando de PlatformIO para compilar y subir el firmware al ESP8266, y observa el "Serial Monitor" a 9600 baudios.

   - Broker MQTT y Control: Configura tu broker MQTT y suscríbete a los siguientes topics:

   - Control: sistema/luz/control

   - Datos: sistema/luz/ambiente

   - Estado: sistema/luz/estado

- Envía los comandos:

   - "ON": Fuerza el encendido del LED (reenvía "LED_ON" vía LoRa).

   - "OFF": Fuerza el apagado del LED (reenvía "LED_OFF").

   - "AUTO": Vuelve al modo automático para que el ESP32 controle el LED según la luminosidad.
 

## imagenes:
<p align="center">
  <img src="Modulo, 2 lora punto a punto + mqtt/Imagenes/Vscode.jpeg" alt="VSCODE" width="600">
</p>  
   
<p align="center">
  <img src="Modulo, 2 lora punto a punto + mqtt/Imagenes//Json_Raw.jpeg" alt="Broker con Json" width="600">
</p>    
   

<p align="center">
  <img src="Modulo, 2 lora punto a punto + mqtt/Imagenes//APK__Iot-MqTT-Panel.jpeg" alt="APK con datos json visuales" width="600">
</p>


---  


## Video de Funcionamiento

<p align="center">
  <a href="https://youtube.com/shorts/NL2lv98-YO0?feature=share">
    <img src="C%20prototipo/Imagenes/VideoPreview.png" alt="Ver video en YouTube" width="600">
  </a>
</p>
