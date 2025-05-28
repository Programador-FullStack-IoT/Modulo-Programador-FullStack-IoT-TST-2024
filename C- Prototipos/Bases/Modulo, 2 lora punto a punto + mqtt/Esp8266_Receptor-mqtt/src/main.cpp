#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <LoRa.h>
#include <time.h>
#include <ArduinoJson.h>  // Necesaria para StaticJsonDocument y serializeJson

// Configuración WiFi
const char* ssid = "SSID";
const char* password = "PASSS";

// Configuración MQTT
const char* mqtt_server = "144b0f0b708947df835498a9bf10bfb5.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqtt_user = "ispc2025";
const char* mqtt_password = "ispc2025A";

// Topics MQTT
const char* data_topic = "sistema/luz/ambiente";
const char* control_topic = "sistema/luz/control";
const char* status_topic = "sistema/luz/estado";

// Pines LoRa
#define LORA_SS    15
#define LORA_RST   0
#define LORA_DIO0  5

// Pin para el LED del ESP8266 (control local)
#define LED_REMOTO_PIN 2  // Equivalente a D4

// Objetos para WiFi y MQTT
WiFiClientSecure espClient;
PubSubClient client(espClient);

// Variables de estado
float lux = 0.0;
int luzAmbientePorc = 0;
bool luzRefuerzo = false;  // Estado actual del LED
bool modoForzado = false;  // Si se ha forzado el estado manualmente
unsigned long ultimoComando = 0;

#define MSG_BUFFER_SIZE (256)
char msg[MSG_BUFFER_SIZE];

// Declaraciones adelantadas de funciones
void restartWithMessage(const char* reason);
void setup_wifi();
void enviarDatos();
String getTimestamp();
void reconnect();

// Reinicia el ESP8266 mostrando el mensaje de error
void restartWithMessage(const char* reason) {
  Serial.println(reason);
  delay(2000);
  ESP.restart();
}

// Conexión a WiFi
void setup_wifi() {
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  int wifi_attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    wifi_attempts++;
    if (wifi_attempts > 20) {
      restartWithMessage("No se pudo conectar a WiFi. Reiniciando...");
    }
  }

  Serial.println("\nWiFi conectado. IP: ");
  Serial.println(WiFi.localIP());
}

// Obtener timestamp formateado
String getTimestamp() {
  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);
  char buf[25];
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", timeinfo);
  return String(buf);
}

// Enviar datos formateados vía MQTT
void enviarDatos() {
  String timestamp = getTimestamp();
  StaticJsonDocument<256> doc;
  
  doc["timestamp"] = timestamp;
  doc["lux"] = lux;
  doc["porcentaje"] = luzAmbientePorc;
  doc["estado_luz"] = luzRefuerzo ? "on" : "off";
  doc["modo"] = modoForzado ? "forzado" : "automatico";
  doc["rssi"] = WiFi.RSSI();
  doc["ip"] = WiFi.localIP().toString();

  serializeJson(doc, msg);
  client.publish(data_topic, msg);
  
  Serial.print("Datos enviados: ");
  Serial.println(msg);
}

// Función de callback para procesar mensajes MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido en topic: ");
  Serial.println(topic);

  // Convertir payload a String
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  // Procesar comandos de control recibidos en el topic control_topic
  if (strcmp(topic, control_topic) == 0) {
    if (message.equalsIgnoreCase("ON")) {
      // Envío del comando LoRa para forzar el LED del ESP32 encendido
      LoRa.beginPacket();
      LoRa.print("LED_ON");
      LoRa.endPacket();
      
      luzRefuerzo = true;
      modoForzado = true;
      ultimoComando = millis();
      Serial.println("Comando ON recibido y reenviado a ESP32");
    } 
    else if (message.equalsIgnoreCase("OFF")) {
      // Envío del comando LoRa para forzar el LED del ESP32 apagado
      LoRa.beginPacket();
      LoRa.print("LED_OFF");
      LoRa.endPacket();
      
      luzRefuerzo = false;
      modoForzado = true;
      ultimoComando = millis();
      Serial.println("Comando OFF recibido y reenviado a ESP32");
    }
    else if (message.equalsIgnoreCase("AUTO")) {
      // Modo automático: se desactiva el forzado y se vuelve al control automático
      modoForzado = false;
      ultimoComando = millis();
      Serial.println("Comando AUTO recibido");
    }
    
    // Actualizar LED local para reflejar el estado del control forzado
    digitalWrite(LED_REMOTO_PIN, luzRefuerzo ? HIGH : LOW);
    enviarDatos();
  }
}

// Reconexión al broker MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Intentando conexión MQTT...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), mqtt_user, mqtt_password)) {
      Serial.println("Conectado al servidor MQTT");
      client.subscribe(control_topic);
      client.publish(status_topic, "ESP8266 conectado");
    } else {
      Serial.print("Falló, rc=");
      Serial.print(client.state());
      Serial.println(" intentando de nuevo en 5 segundos");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_REMOTO_PIN, OUTPUT);
  digitalWrite(LED_REMOTO_PIN, LOW);

  setup_wifi();
  
  // Configuración de la hora mediante NTP
  configTime(0, 0, "pool.ntp.org");
  Serial.println("Obteniendo hora...");
  while (time(nullptr) < 100000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nHora configurada");

  // Inicialización de LoRa
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  if (!LoRa.begin(433E6)) {
    restartWithMessage("Error al iniciar LoRa");
  }
  Serial.println("LoRa iniciado correctamente");

  // Configuración de MQTT
  espClient.setInsecure(); // Se omite la verificación SSL
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  // Reconectar a MQTT si se pierde la conexión
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Verificar si se reciben datos vía LoRa (por ejemplo, la lectura de luminosidad enviada por el ESP32)
  if (LoRa.parsePacket()) {
    String datos = LoRa.readString();
    if (datos.startsWith("LUX=")) {
      lux = datos.substring(4).toFloat();
      luzAmbientePorc = constrain(map(lux, 0, 500, 0, 100), 0, 100);

      // Si no estamos en modo forzado, se controla automáticamente el LED local según la luminosidad
      if (!modoForzado) {
        luzRefuerzo = (lux < 50);
        digitalWrite(LED_REMOTO_PIN, luzRefuerzo ? HIGH : LOW);
      }

      enviarDatos();
    }
  }

  // Vuelve a modo automático si no se recibe cambio de comando forzado durante 1 minuto
  if (modoForzado && millis() - ultimoComando > 60000) {
    modoForzado = false;
    Serial.println("Volviendo a modo automático por inactividad");
  }

  delay(100);
}
