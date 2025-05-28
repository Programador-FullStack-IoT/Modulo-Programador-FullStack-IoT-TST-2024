#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <BH1750.h>

// Pines LoRa
#define LORA_SS    5
#define LORA_RST   14
#define LORA_DIO0  2
#define LORA_SCK   18
#define LORA_MISO  19
#define LORA_MOSI  23

// Pines LED
#define LED_SECUNDARIO 16     // LED controlado manualmente o por luz baja
#define LED_AMBIENTAL  17     // LED con brillo progresivo (PWM)

// PWM
#define PWM_CANAL 0
#define PWM_FREQ  5000
#define PWM_RES   8  // 0–255

BH1750 lightSensor;

// Estado de control
bool forzarLED = false;
unsigned long ultimoComandoManual = 0;
const unsigned long TIEMPO_FORZADO = 60000; // 1 minuto
bool avisoAutomaticoEnviado = false;

void setup() {
  Serial.begin(9600);

  pinMode(LED_SECUNDARIO, OUTPUT);
  digitalWrite(LED_SECUNDARIO, LOW);

  // PWM LED ambiental
  ledcSetup(PWM_CANAL, PWM_FREQ, PWM_RES);
  ledcAttachPin(LED_AMBIENTAL, PWM_CANAL);
  ledcWrite(PWM_CANAL, 0);

  // BH1750
  Wire.begin();
  if (!lightSensor.begin()) {
    Serial.println("❌ Fallo al iniciar BH1750");
    while (true);
  }
  Serial.println("✅ BH1750 iniciado");

  // LoRa
  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  if (!LoRa.begin(433E6)) {
    Serial.println("❌ Fallo al iniciar LoRa. Revisa los pines.");
    while (true);
  }
  Serial.println("✅ LoRa iniciado. Esperando comandos...");
}

void loop() {
  // Comandos LoRa
  if (LoRa.parsePacket()) {
    String comando = LoRa.readString();
    Serial.print("📥 Comando recibido: ");
    Serial.println(comando);

    if (comando == "LED_ON") {
      forzarLED = true;
      digitalWrite(LED_SECUNDARIO, HIGH);
      ultimoComandoManual = millis();
      avisoAutomaticoEnviado = false;
      Serial.println("🔔 LED secundario forzado ON");
    } 
    else if (comando == "LED_OFF") {
      forzarLED = false;
      digitalWrite(LED_SECUNDARIO, LOW);
      ultimoComandoManual = millis();
      avisoAutomaticoEnviado = false;
      Serial.println("🔕 LED secundario forzado OFF");
    }
  }

  // Verificación de timeout sin comandos
  if (forzarLED && (millis() - ultimoComandoManual > TIEMPO_FORZADO) && !avisoAutomaticoEnviado) {
    forzarLED = false;
    avisoAutomaticoEnviado = true;
    Serial.println("⏳ Sin comandos por 1 minuto → Volviendo a modo automático");
    LoRa.beginPacket();
    LoRa.print("INFO: LED secundario vuelve a automático");
    LoRa.endPacket();
  }

  // Sensor de luz
  static unsigned long ultimoEnvio = 0;
  if (millis() - ultimoEnvio > 3000) {
    float lux = lightSensor.readLightLevel();
    Serial.print("🔆 Luminosidad: ");
    Serial.print(lux);
    Serial.println(" lux");

    // Enviar valor por LoRa
    String datos = "LUX=" + String(lux, 1);
    LoRa.beginPacket();
    LoRa.print(datos);
    LoRa.endPacket();
    Serial.println("📤 Datos enviados: " + datos);

    // PWM para luz ambiental (0 lux = 100%, 100+ lux = 0%)
uint8_t brilloPWM = map(constrain(lux, 0, 100), 0, 100, 255, 0);
ledcWrite(PWM_CANAL, brilloPWM);

// Mostrar porcentaje
float porcentaje = (brilloPWM / 255.0) * 100;
Serial.print("💡 LED ambiental al ");
Serial.print(porcentaje, 1);
Serial.println("% de brillo");


    // Control automático del LED secundario
    if (!forzarLED) {
      if (lux < 10) {
        digitalWrite(LED_SECUNDARIO, HIGH);
        Serial.println("🌑 Luz muy baja → LED secundario ON (automático)");
      } else {
        digitalWrite(LED_SECUNDARIO, LOW);
        Serial.println("🌓 Luz suficiente → LED secundario OFF (automático)");
      }
    }

    ultimoEnvio = millis();
  }
}
