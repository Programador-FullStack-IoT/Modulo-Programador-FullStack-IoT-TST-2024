#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <LoRa.h>


// Pines LoRa
#define LORA_SCK 18
#define LORA_MISO 19
#define LORA_MOSI 23
#define LORA_SS 5
#define LORA_RST 14
#define LORA_DIO0 26

#define RELAY_PIN 27  // Simula el relay con un LED

// WiFi y MQTT
const char* ssid = "Fibertel-WiFi032";
const char* password = "vuelalto@67";
const char* mqtt_server = "telecomunicaciones.ddns.net";  // o tu IP/localhost
const int mqtt_port = 2480;
const char* mqtt_topic = "omnisens/datos/AQC_001";

WiFiClient espClient;
PubSubClient client(espClient);

//IPAddress local_IP(192, 168, 0, 75);
//IPAddress gateway(192, 168, 0, 1);
//IPAddress subnet(255, 255, 255, 0);

void connectWiFi() {
  
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado");
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando a MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("conectado!");
    } else {
      Serial.print("falló, rc=");
      Serial.print(client.state());
      Serial.println(" intentando de nuevo en 5s");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial);

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Apagado al inicio

  // Configurar LoRa
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(433E6)) {
    Serial.println("Fallo al iniciar LoRa");
    while (1);
  }

  Serial.println("Nodo Receptor iniciado...");

   // WiFi + MQTT
  //WiFi.config(local_IP, gateway, subnet);
  connectWiFi();
  client.setServer(mqtt_server, mqtt_port);
  client.connect("ESP32Client");
}

void loop() {


  client.loop();

  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String incoming = "#";
    while (LoRa.available()) {
      incoming += (char)LoRa.read();
    }

    Serial.print("Mensaje recibido: ");
    Serial.println(incoming);

    // PUBLICAR MENSAJE A MQTT
    client.publish(mqtt_topic, incoming.c_str());

    // Analizar datos. Supongamos que recibimos: "Temp:31 Hum:60"
    if (incoming.indexOf("Temp:") != -1) {
      int tempIndex = incoming.indexOf("Temp:") + 5;
      int spaceIndex = incoming.indexOf(' ', tempIndex);
      String tempStr = incoming.substring(tempIndex, spaceIndex);
      float temp = tempStr.toFloat();

      if (temp > 30) {
        digitalWrite(RELAY_PIN, HIGH); // Activa relay
        Serial.println(">>> Relay ON (Temp alta)");
      } else {
        digitalWrite(RELAY_PIN, LOW);
        Serial.println(">>> Relay OFF (Temp normal)");
      }
    }
  }
}
