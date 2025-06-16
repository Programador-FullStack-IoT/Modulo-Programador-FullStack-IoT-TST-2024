#include <Arduino.h>
#include <RadioLib.h>  // Usamos RadioLib en lugar de LoRaLib
#include <SPI.h>
#include <Wire.h>
#include "MQ135Sensor.h"
#include "AHT25Sensor.h"
#include "BMP280Sensor.h"
#include "LDRSensor.h"
#include "SalidaPWM.h"
#include "SalidasRele.h"
#include "SensorData.h"
#include "DatoJson.h"
#include <driver/timer.h>

// --- Configuración Hardware ---
// Pines LoRa (ESP32)
#define LORA_SS    5
#define LORA_RST   14
#define LORA_DIO0  26
#define LORA_DIO1  34  // Opcional para interrupciones

// Pines Sensores/Actuadores
#define MQ135_PIN  39
#define PWM_PIN    32
#define RELE_1     33
#define RELE_2     25
#define WS2812_PIN 27
#define LDR_PIN    36

// --- Instancias Globales ---
// Sensores
MQ135Sensor mq135(MQ135_PIN);
AHT25Sensor aht25;
BMP280Sensor bmp280;
LDRSensor ldr(LDR_PIN);

// Actuadores
SalidaPWM vel_motor(PWM_PIN);
SalidasRele salidas(RELE_1, RELE_2);

// LoRa (RadioLib)
SX1278 radio = new Module(LORA_SS, LORA_DIO0, LORA_RST, LORA_DIO1);  // ¡Asegúrate de conectar DIO1!

// Datos y Comunicación
const String NODE_ID = "AQC_001";
SensorData sensorData(NODE_ID, &mq135, &bmp280, &aht25, &ldr, &salidas, &vel_motor);
DatoJson datoJson;

// Ticker para envío periódico
//Ticker envioTicker;
volatile bool enviarFlag = false;
hw_timer_t *timer = NULL;

void IRAM_ATTR onTimer() {
  enviarFlag = true;  // Marca el flag para enviar datos
}


// --- Setup ---
void setup() {

  Serial.begin(115200);
  while (!Serial && millis() < 5000); // Timeout de 5 segundos para Serial

  Serial.println("\n[Nodo Sensor] Inicializando...");

  // Inicializar I2C para sensores
  Wire.begin();
  if (!aht25.begin()) {
    Serial.println("[ERROR] Fallo al iniciar AHT25");
  }
  if (!bmp280.begin()) {
    Serial.println("[ERROR] Fallo al iniciar BMP280");
  }

  //inicializar actuadores
  vel_motor.begin();
  salidas.begin();

  // Inicializar SPI para LoRa
  SPI.begin(18, 19, 23, LORA_SS);

  // Configurar Radio (Clase C)
  int state = radio.begin(433.0, 125.0, 9, 5, 0xF3, 17);
  if (state != RADIOLIB_ERR_NONE) {
    Serial.print("[LoRa] Error inicializando: ");
    Serial.println(state);
    while (true) { delay(100); } // Bloqueo controlado
  }
  //radio.setRfSwitchPins(-1, -1);
  Serial.println("[LoRa] Inicializado (Clase C)");

  // Configurar ticker para envío cada 5 segundos
  //envioTicker.attach(5.0, setEnviarFlag);
  //Serial.println("[Ticker] Configurado para envío cada 5 segundos");

  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 5000000, true);  // 5 segundos
  timerAlarmEnable(timer);
  Serial.println("[Timer] Configurado para envío cada 5 segundos");

  radio.setRfSwitchPins(-1, -1);  // Deshabilitar control de antena si no se usa
  radio.setCurrentLimit(100);     // Establecer corriente máxima a 100mA
  
  Serial.println("[Sistema] Nodo sensor listo");

}

// --- Loop Principal ---
void loop() {
  // 1) Enviar datos periódicos
  if (enviarFlag) {
    enviarFlag = false;
    Serial.println("[LoRa] Activado envío de datos");

    String json = sensorData.toJSON();
    Serial.println("[LoRa] JSON preparado: " + json);

    // Verificar tamaño del JSON (máx. 255 bytes con RadioLib)
    if (json.length() > 240) {
      Serial.println("[ERROR] JSON demasiado grande");
      return;
    }

    // Enviar con timeout
    unsigned long startTime = millis();
    int state = RADIOLIB_ERR_UNKNOWN;

    while (millis() - startTime < 1000) { // Timeout de 1 segundo
      state = radio.transmit(json);
      if (state == RADIOLIB_ERR_NONE) break;
      delay(100);
    }

    if (state != RADIOLIB_ERR_NONE) {
      Serial.print("[LoRa] Error en TX: ");
      Serial.println(state);
    } else {
      Serial.println("[LoRa] Datos enviados con éxito");
    }
  }


 // 2) Escucha continua (Clase C)
  String respuesta;
  int state = radio.receive(respuesta);
  
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println("[LoRa] Orden recibida: " + respuesta);
    
    if (respuesta.length() > 0 && respuesta.indexOf("{") >= 0) {
        uint8_t pwm;
        bool r1, r2;
        uint8_t codAlarma;
        
        if (datoJson.procesarOrdenes(respuesta, NODE_ID, pwm, r1, r2, codAlarma)) {
            vel_motor.comandoPWM(pwm);
            salidas.setRele1(r1);
            salidas.setRele2(r2);
            Serial.println("[Acciones] Comandos ejecutados");
            
            // Enviar confirmación (opcional)
            //String ack = "{\"node\":\"" + NODE_ID + "\",\"status\":\"OK\"}";
            //radio.transmit(ack);
        }
    } else {
        Serial.println("[LoRa] Mensaje inválido recibido");
    }
  }

  // Pequeño delay para evitar sobrecarga
  delay(10);
}