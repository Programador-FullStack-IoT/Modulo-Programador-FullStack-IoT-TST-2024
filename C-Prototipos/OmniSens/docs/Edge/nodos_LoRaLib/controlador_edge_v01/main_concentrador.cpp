//  // control_mqtt/main.cpp (versión bidireccional de prueba)
// #include <Arduino.h>
// #include <WiFi.h>
// #include <PubSubClient.h>
// #include <SPI.h>
// #include <LoRa.h>
// //#include <ArduinoJson.h>

// // — Datos WiFi/MQTT —
// const char* WIFI_SSID     = "Fibertel-WiFi032";
// const char* WIFI_PASS     = "vuelalto@67";
// const char* MQTT_SERVER   = "telecomunicaciones.ddns.net";
// const int   MQTT_PORT     = 2480;
// const char* TOPIC_DATOS   = "omnisens/datos/AQC_001";
// const char* TOPIC_COMANDOS= "omnisens/comandos/AQC_001";

// // — Pines LoRa —
// #define LORA_CS   5
// #define LORA_RST  14
// #define LORA_INT  26

// //LoRaComm lora(433E6);
// WiFiClient espClient;
// PubSubClient mqtt(espClient);

// // Callback LoRa (opcional, si quieres confirmar recepción RX<–TX)
// volatile bool sendLoRaFlag = false;
// String pendingLoRaCmd = "";

// volatile bool loraFlag = false;
// String   loraMsg;

// void onReceive(int packetSize) {
//   if (packetSize == 0) return;
//   String incoming;
//   while (LoRa.available()) incoming += (char)LoRa.read();
//   loraMsg  = incoming;
//   loraFlag = true;
// }

// void mqttCallback(char* topic, byte* payload, unsigned int len) {
//   String cmd;
//   for (unsigned int i=0; i<len; i++) cmd += (char)payload[i];
//   Serial.println("[MQTT] Comando: " + cmd);

//   /* // Reenviar por LoRa
//   LoRa.beginPacket();
//     LoRa.print(cmd);
//   LoRa.endPacket();
//   delay(100); // Esperar un poco para asegurar que se envía correctamente
//   //Serial.println("[LoRa] Comando reenviado");
//   LoRa.receive(); */
//   pendingLoRaCmd = cmd; // Guardar comando pendiente
//   sendLoRaFlag = true; // Marcar que hay un comando pendiente para enviar por LoRa
// }




// void setup() {
//   Serial.begin(115200);
//   delay(500);
//   Serial.println("\n[Nodo MQTT] Inicializando…");

//   // WiFi
//   WiFi.begin(WIFI_SSID, WIFI_PASS);
//   while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
//   Serial.println("\n[WiFi] Conectado, IP: " + WiFi.localIP().toString());

//   // MQTT
//   mqtt.setServer(MQTT_SERVER, MQTT_PORT);
//   mqtt.setCallback(mqttCallback);

//   // LoRa
//   SPI.begin(18,19,23,LORA_CS);
//   LoRa.setPins(LORA_CS, LORA_RST, LORA_INT);
//   if (!LoRa.begin(433E6)) Serial.println("[LoRa] ERROR"); 
//   else Serial.println("[LoRa] OK");

//   LoRa.setSpreadingFactor(12);
//   LoRa.setSignalBandwidth(125E3);
//   LoRa.setCodingRate4(5);
//   //LoRa.onReceive(onReceive);
//   LoRa.receive();
//   Serial.println("[LoRa] Inicializado");

//   Serial.println("[Sistema] Gateway listo");
// }

// void reconnectMQTT() {
//   while (!mqtt.connected()) {
//     Serial.print("[MQTT] Reconectando...");
//     if (mqtt.connect("control_mqtt")) {
//       Serial.println("OK");
//       mqtt.subscribe(TOPIC_COMANDOS);
//     } else {
//       Serial.print("ERR, rc=");
//       Serial.print(mqtt.state());
//       Serial.println(" -> retry en 5s");
//       delay(5000);
//     }
//   }
// }

// void loop() {
//   // MQTT reconnect
//   if (!mqtt.connected()) reconnectMQTT();
//   mqtt.loop();

//    //  Si deseas publicar paquetes recibidos por LoRa a MQTT
//    if (sendLoRaFlag) {
//     sendLoRaFlag = false;


//       LoRa.beginPacket();
//       LoRa.print(pendingLoRaCmd);

//       if (LoRa.endPacket(false)) {
//         Serial.println("[LoRa] Comando enviado: " + pendingLoRaCmd);
//       }
//       else {
//         Serial.println("[LoRa] ERROR al enviar comando");
//       }
//       delay(200); // Esperar un poco para asegurar que se envía correctamente
    
//     LoRa.receive(); // Volver a modo escucha
//     Serial.println("[LoRa] Volviendo a modo escucha");
//   }

//    int packetSize = LoRa.parsePacket();
//   if (packetSize > 0) {
//     String mensaje;
//     while (LoRa.available())
//       mensaje += (char)LoRa.read();
//     Serial.println("[LoRa] Recibido: " + mensaje);
//     mqtt.publish(TOPIC_DATOS, mensaje.c_str());
//   }

// } 

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <LoRa.h>

// — Configuración Wi-Fi / MQTT —
const char* WIFI_SSID      = "Fibertel-WiFi032";
const char* WIFI_PASS      = "vuelalto@67";
const char* MQTT_SERVER    = "telecomunicaciones.ddns.net";
const uint16_t MQTT_PORT   = 2480;
const char* TOPIC_COMANDOS = "omnisens/comandos/AQC_001";
const char* TOPIC_DATOS    = "omnisens/datos/AQC_001";    // publica datos

// — Pines LoRa —
#define LORA_SS    5
#define LORA_RST   14
#define LORA_DIO0  26

WiFiClient   netClient;
PubSubClient mqtt(netClient);

// Flag + buffer para el comando pendiente
volatile bool sendFlag = false;
String        pendingCmd;

// Callback de MQTT: sólo almacena el mensaje
void mqttCallback(char* topic, byte* payload, unsigned int len) {
  String cmd;
  for (unsigned int i = 0; i < len; i++) cmd += (char)payload[i];
  Serial.println("[MQTT] Recibido orden: " + cmd);
  pendingCmd = cmd;
  sendFlag   = true;
}

void reconnectMQTT() {
  while (!mqtt.connected()) {
    Serial.print("[MQTT] Reconectando…");
    if (mqtt.connect("gateway_test")) {
      Serial.println("OK");
      mqtt.subscribe(TOPIC_COMANDOS);
      Serial.println("[MQTT] Subscrito a " + String(TOPIC_COMANDOS));
    } else {
      Serial.print("ERR rc=");
      Serial.print(mqtt.state());
      Serial.println(" → retry en 3s");
      delay(3000);
    }
  }
}


void setup() {
  Serial.begin(115200);
  while (!Serial);

  // 1) Conecta Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("[WiFi] Conectando");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\n[WiFi] OK, IP: " + WiFi.localIP().toString());

  // 2) Inicia MQTT
  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
  mqtt.setCallback(mqttCallback);
  reconnectMQTT();  // reconectar al iniciar

  // 3) Inicia LoRa
      // Inicialización manual de SPI (por seguridad)
    SPI.begin(18, 19, 23, LORA_SS);

    // Seteo explícito de pines (aunque LoRaComm ya los usa por defecto)
    LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
    Serial.println("[Sistema] Pines LoRa configurados");
    Serial.println("[Sistema] Inicializando modulo comunicacion LoRa ...");

    if (LoRa.begin(433E6)) {
        Serial.println("[Sistema]LoRa inicializado correctamente");
    } else {
        Serial.println("[Sistema] Fallo al iniciar LoRa");
    }
    
    LoRa.setSpreadingFactor(12);
    LoRa.setSignalBandwidth(125E3);
    LoRa.setCodingRate4(5);
    LoRa.setTxPower(20); // Ajusta la potencia de transmisión según sea necesario
    LoRa.enableCrc(); // Habilita CRC para mayor fiabilidad
    //LoRa.onReceive(onReceive);
    LoRa.receive();  // listo para reci
  Serial.println("[Sistema] Gateway listo");
}

void loop() {
  // 1) Mantener vivo MQTT
  if (!mqtt.connected()) reconnectMQTT();
    
 

  // 2) ¿Tenemos un comando pendiente? Lo enviamos por LoRa aquí, en loop()
  if (sendFlag) {
    sendFlag = false;
    Serial.println("[Sistema] Enviando comando por LoRa: " + pendingCmd);

    // TX bloqueante
    
    LoRa.beginPacket();
    
    LoRa.print(pendingCmd);
    
    LoRa.endPacket(false);
    
    // Pequeño margen antes de volver a RX
    delay(150);
    LoRa.receive();
    Serial.println("[LoRa] Envío completado");
    Serial.println("[LoRa] Módulo en modo RX");
  }
  // 3) ¿Hay datos entrantes por LoRa?
  int packetSize = LoRa.parsePacket();
  if (packetSize > 0) {
    String incoming;
    while (LoRa.available()) {
      incoming += (char)LoRa.read();
    }
    Serial.println("[LoRa] Datos recibidos: " + incoming);
    mqtt.publish(TOPIC_DATOS, incoming.c_str());
  }
   mqtt.loop();

}
