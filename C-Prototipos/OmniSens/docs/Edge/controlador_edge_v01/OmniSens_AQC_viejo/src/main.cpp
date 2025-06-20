#include <Arduino.h>
#include <Wire.h>
#include "MQ135Sensor.h"
#include "AHT25Sensor.h"
#include "BMP280Sensor.h"
#include "WS2812Controller.h"
#include "LDRSensor.h"
#include "SalidaPWM.h"
#include "SalidasRele.h"

// --------------------- Configuración de Pines ---------------------
#define MQ135_PIN 34 // GPIO34 (ADC1_CH6)
#define PWM_PIN 4 // TOUCH0 GPIO4
#define RELE_1 0 // TOUCH1 GPIO0
#define RELE_2 2 // TOUCH2 GPIO2
#define WS2812_PIN 39  // Pin para la tira WS2812
#define LDR_PIN 36 // Pin entrada LDR


// --------------------- Inicialización de Sensores y Periféricos ---------------------
MQ135Sensor mq135(MQ135_PIN);
AHT25Sensor aht25;
BMP280Sensor bmp280;
WS2812Controller baliza(WS2812_PIN);
LDRSensor ldr(LDR_PIN);
SalidaPWM vel_motor(PWM_PIN);
SalidasRele salidas(RELE_1, RELE_2);
EnlaceLora com_lora();
DatoJson envio;
DatoJson recibo;  

// Variables globales para el menú
int menuActual = 0;
const int numMenus = 3;

// --------------------- Configuración Inicial ---------------------
void setup() {
    Serial.begin(115200);
    while (!Serial);

    Wire.begin();
    
    mq135.begin();
    aht25.begin();
    bmp280.begin();
    ldr.begin();
    vel_motor.begin();
    salidas.begin();
    com_lora.begin();
    
    
    baliza.begin();
    
   
}

// --------------------- Lógica del Menú ---------------------
void navegarMenu() {
    int touchPressed = touch.checkTouch();
    
    if (touchPressed == 1) { // Next
        menuActual = (menuActual + 1) % numMenus;
        lcd.displayMenu(menuActual);
        lcdParallel.displayMenu(menuActual);
    } 
    else if (touchPressed == 2) { // Previous
        menuActual = (menuActual - 1 + numMenus) % numMenus;
        lcd.displayMenu(menuActual);
        lcdParallel.displayMenu(menuActual);
    }
}

// --------------------- Bucle Principal ---------------------
void loop() {
    navegarMenu();

    // Lectura de Sensores
    float mq135Data = mq135.readFilteredData();
    float ahtTemp, ahtHumidity;
    aht25.readData(ahtTemp, ahtHumidity);
    float bmpTemp, bmpPressure;
    bmp280.readData(bmpTemp, bmpPressure);
    float lightIntensity = bh1750.leerLux();
    float indiceUV = ml8511.leerUV();

    // Actualizar el menú y mostrar en el LCD
    lcd.updateMenu(menuActual, mq135Data, ahtTemp, ahtHumidity, bmpTemp, bmpPressure, lightIntensity, indiceUV);
    lcdParallel.updateMenu(menuActual, mq135Data, ahtTemp, ahtHumidity, bmpTemp, bmpPressure, lightIntensity, indiceUV);

    // Actualizar la tira LED según el AQI calculado (esto es un ejemplo)
    int aqi = (int)(mq135Data); // Simplificación del AQI usando solo MQ135
    ws2812.updateLED(aqi);

    delay(1000); // Esperar 1 segundo antes de la siguiente lectura
}
