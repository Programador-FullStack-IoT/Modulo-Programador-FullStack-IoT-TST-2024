#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

// Pines LoRa (ajusta si usas otros)
#define LORA_SS    5
#define LORA_RST  14
#define LORA_DIO0 26

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // SPI + LoRa init
  SPI.begin(18, 19, 23, LORA_SS);
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  if (!LoRa.begin(433E6)) {
    Serial.println("ERROR: no se pudo iniciar LoRa");
    while (1);
  }
  LoRa.setSpreadingFactor(12);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);

  // Opción: ignorar CRC para ver tramas corruptas
  LoRa.enableCrc();  // o LoRa.disableCrc();

  Serial.println("== LoRa Sniffer iniciado ==");

  // No enviar nada, solo escuchar
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize > 0) {
    Serial.print("[Sniffer] Paquete capturado (bytes=");
    Serial.print(packetSize);
    Serial.println("):");

    String payload;
    while (LoRa.available()) {
      char c = (char)LoRa.read();
      payload += c;
    }
    // Imprime hex para ver si contiene datos no ASCII
    Serial.print("  Payload: ");
    Serial.println(payload);

    // También muestra RSSI y SNR
    Serial.print("  RSSI: ");
    Serial.print(LoRa.packetRssi());
    Serial.print(" dBm, SNR: ");
    Serial.print(LoRa.packetSnr());
    Serial.println(" dB");
    Serial.println();
  }
}
