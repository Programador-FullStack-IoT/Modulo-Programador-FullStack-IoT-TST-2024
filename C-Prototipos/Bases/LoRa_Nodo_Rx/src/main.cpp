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
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String incoming = "";
    while (LoRa.available()) {
      incoming += (char)LoRa.read();
    }

    Serial.print("Mensaje recibido: ");
    Serial.println(incoming);

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
