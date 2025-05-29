# Controlador Edge con comunicacion LoRa.



## Descripción General

Este controlador tiene como objetivo adquirir datos de ambiente mediante una serie de sensores, hacer un preprocesamiento de los mismos, enviarlos a una nube para su almacenamiento y analisis, y ejecutar acciones sobre una serie de actuadores basado en procesamiento directo en el dispositivo para variables criticas, asi como tambien ejecutar ordenes recibidas desde un servidor remoto.  
El sistema esta diseñado en torno a un microcontrolador **ESP32**, utiliza sensores para medir la concentración de gases, la intensidad de luz, la temperatura, la presion atmosferica y la humedad. Además, cuenta con una salida PWM para controlar la velocidad de un extractor o ventilador, y dos salidas a rele para comando general.   
Para el apartado de visualizacion e interfaz se cuenta con una baliza y buzzer para indicar estados de alerta de forma intuitiva. 
El controlador debe permitir la configuracion local de parametros como los sensores activos, nombre de las salidas y su ID para individualizarlo todo mediante un micrositio web local servido por wifi.   


### Componentes:

1. **Sensores:**
   - **MQ135**: Medición de la concentración de gases (CO2, NOx, etc.).
   - **AHT25**: Medición de temperatura y humedad.
   - **BMP280**: Medición de temperatura y presión barométrica.
   - **LDR**: Sensor de intensidad de luz.
  

2. **Visualizadores**
   - **Baliza de tres colores**: Visualización de estados y alertas.    

3. **Actuadores**
   - **Salida PWM**: Para manejar velocidad de motores.  
   - **Salidas a Relé**: Para manejo de cargas en "todo/nada"    

4. **Interfaz de comunicación**
   - **Modulo LoRa SX1278**: Envio y recepcion de datos por enlace de radio.  
   - **Microservidor web**: Configuracion de parametros del controlador 



## Estructura del Proyecto

lib/       
  AHT25/   
    ├── AHT25Sensor.cpp   
    ├── AHT25Sensor.h    
  LDR/   
    ├── LDRSensor.cpp   
    ├── LDRSensor.h      
  BMP280/   
    ├── BMP280Sensor.cpp   
    ├── BMP280Sensor.h      
  MQ135/   
    ├── MQ135Sensor.cpp   
    ├── MQ135Sensor.h     
  SalidaPWM/   
    ├── PWMController.cpp   
    ├── PWMController.h    
  BALIZA/   
    ├── WS2812Controller.cpp   
    ├── WS2812Controller.h         
  SalidasRele/   
    ├── salidarele.cpp   
    ├── salidaRele.h   
  EnlaceLoRa/   
    ├── ComLora.cpp   
    ├── ComLora.h      
  DatoJson/   
    ├── DatoJson.cpp   
    ├── DatoJson.h      


src/  
├── main.cpp

  

## Estructura del Código

### 1. **Archivo `main.cpp`:**

Este es el archivo principal que inicializa los sensores, el LCD, los touchpads y la tira LED. El bucle principal realiza lecturas de los sensores, actualiza el menú del LCD, y controla la tira LED en función del AQI calculado.

#### **Configuración de Pines:**   
A continuacion se muestra un diagrama con la distribucion de pines en el modulo ESP32-Wroom de **Nodemcu**
  
<center><img src="./assets/esp32nodemcu.jpg"></center>    

**Asignacion**   

- **MQ135** (sensor analógico): Pin ADC0 (GIOP34, pata 3)   
- **LDR** (sensor analogico): Pin ADC5 (GIOP33, pata 8)
- **Salidas**: PWM, Rele_1 y Rele_2 (GIOP4/pata 26, GIOP0/pata 25 y GIOP2/pata 24)  
- **tira LED WS2812**: Pin GIOP39 / pata 4
- **Dispositivos I2C**: Pin I2C SDA / pata 33 y Pin I2C SCL / 36  
  
  
#### **Inicialización de Sensores y Periféricos:**
- En el `setup()`, se inicializan todos los sensores y periféricos y la comunicación LoRa. Aqui se controla el correecto funcionamiento de los componentes del sistema.  
- Se testea si se esta activo un jumper de configuración, de ser asi, se inicia un microservidor web para cargar parametros del controlador.
  
#### **Lógica de control:**  
Una vez finalizada la funcion `setup()` se inicia el funcionamiento del controlador propiamente dicho en la funcion `loop()`, donde se leen los sensores, se envian paquetes y se atienden comandos por LoRa, se ejecutan los actuadores y las alertas locales en baliza y buzzer. 

#### 1. **Bucle Principal (`loop()`):**
- El bucle principal realiza las siguientes tareas:
  1. **Lectura de Sensores:** Lee los valores de los sensores (MQ135, AHT25, BMP280, LDR). Si falla algun sensor emite una alerta y continua el bucle.
  2. **Forma paquete JSON:** Empaqueta los datos de sensores en un archivo JSON para enviarlo al concentrador por medio de LoRa.
  3. **Lee comandos:** Revisa si se recibio algun comando desde el servidor remoto a traves de LoRa
  3. **Control de Actuadores:** Se realiza control sobre los actuadores en funcion de comandos remotos o procesos locales de baja latencia.
  4. **Ejecucion de alertas:** Representa todas las alertas del sistema accionando la baliza y el buzzer segun el caso.

### 2. **Clase `LDRSensor` (Sensor de Luz):**

#### **Descripción:**
Esta clase maneja un sensor **LDR** para medir la intensidad de luz. 

#### **Métodos:**
- **begin():** Inicializa el sensor LDR.
- **leerLuz():** Lee y devuelve la intensidad de luz.

#### **Uso:**
El sensor LDR se inicializa en el `setup()` y se llama a `leerLuz()` en el bucle principal para obtener la intensidad de luz, luego se debe cargar el valor obtenido en el archivo JSON para enviar.

### 3. **CLase `MQ135Sensor` (Sensor de gases)**  

#### **Descripción:**
Esta clase es la encargada de gestionar el sensor **MQ135** que mide la calidad del aire. El sensor MQ135 puede detectar concentraciones de gas entre 10 y 1000 ppm y es útil para la detección de gases nocivos como amoniaco, dióxido de nitrógeno, alcohol, benceno, dióxido y monóxido de carbono.   

#### **Métodos:**
- **begin():** Inicializa el sensor y configura el filtro de media móvil para suavizar las lecturas.
- **readFilteredData():** Lee los datos del sensor, aplica un filtro de media móvil para suavizar las lecturas y descarta valores fuera de un rango predefinido (10-1000 PPM).   
Devuelve el valor filtrado o -1 si la lectura es inválida.   
    
  
### 4. **Clase `SalidaPWM` (Salida para modulacion PWM):**

#### **Descripción:**
Esta clase maneja una salida PWM para aplicar a la modulacion de velocidad de un motor o extractor. 

#### **Métodos:**
- **begin():** Inicializa el pin de salida y los parametros PWM
- **comandoPWM():** Establece la salida a un porcentaje entre 0% y 100% de duty cycle.

#### **Uso:**
El pin de salida inicializa en el `setup()` y se llama a `comandoPWM()` en el bucle principal para fijar la modulacion PWM en la salida configurada, luego se debe cargar el valor asignado en el archivo JSON para enviar al servidor remoto.   

### 5. **Clase `SalidasRele` (Control de las salidas a rele disponibles):**

#### **Descripción:**
Esta clase maneja las dos salidas a rele disponibles. Permite manejar los pines asignados y reportar el estado de los mismos.

#### **Métodos:**
- **begin():** Inicializa los pines y los mantiene apagados.
- **cambiarEstado(int rele):** cambia el estado de la salida indicada por el argumento y modifica el estado correspondiente para reportar al servidor remoto.  
  

### 6. **Clase `BALIZA` (Tira LED WS2812):**

#### **Descripción:**
Esta clase controla una tira LED **WS2812**, la cual se usa para formar una baliza tricolor para indicar estados y alarmas.

#### **Métodos:**
- **begin():** Inicializa la tira LED, establece cuantos led van a asignarse a cada zona por ejemplo zona_1 = D0, D1, D2; zona_2 = D3, D4, D5; zona_3 = D6; D7.
- **actualizarEstado(int estado):** Cambia el modo de encendido de cada zona para identificar alertas y estados segun una tabla predefinida, de la siguiente manera:
  - **DEFINIR ESTADOS Y ALERTAS:**
  
### 7. **Clase `EnlaceLora` (Control de la comunicacion LoRa):**

#### **Descripción:**
Esta clase controla la comunicacion por **LoRa** , para realizar el envio de datos de los sensores y recibir los comandos desde el servidor a traves del concentrador.

#### **Métodos:**
- 

### 8. **Clase `AHT25Sensor` Sensor de Temperatura y Humedad**

Estos archivos implementan la clase `AHT25Sensor`, responsable de gestionar el sensor **AHT25** que mide temperatura y humedad relativa.

#### **Metodos:**
- **`begin()`:** Inicializa el sensor **AHT25** y verifica su disponibilidad en el bus I2C.  

- **`readData(float &temperature, float &humidity)`:**  Lee los datos de temperatura y humedad desde el sensor.   
Aplicando un filtro que descarta valores fuera de los rangos aceptables.   
Devuelve `true` si los datos son válidos y `false` si no lo son.  
  
### 9. **Clase `BMP280Sensor` Sensor de presion barometrica y temperatura**

Estos archivos implementan la clase `BMP280Sensor`, que gestiona el sensor **BMP280** para la medición de temperatura,  y presión barométrica.

#### **Metodos:**
- **`begin()`:** Inicializa el sensor **BMP280** y verifica su disponibilidad en el bus I2C.  

- **`readData(float &temperature, float &pressure)`:** Lee los datos de temperatura, y presión del sensor.   
Aplica filtros para descartar valores fuera de rangos predefinidos.   
Devuelve `true` si los datos son válidos y `false` si no lo son.  
  
### 10. **Clase `DatoJSON` para manejar archivos JSON de entrada de comandos y salida de datos**


## DEFINIR CLASE MICROSERVIDOR WEB 

  
# ***CONFIGURACIONES***
    
## **Librerias necesarias**

Para que el programa funcione es necesario instalar las siguientes librerias en el IDE:  

- **Adafruit BMP280.h**
- **Adafruit AHTX0.h**
- **MQ135.h** 
- **Adafruit_NeoPixel**   

Para el caso de PlatformIO la mismas pueden instalarse desde el gestor de librerias buscando el nombre de cada una, y haciendo click en `install`.  

### 2. **Configuración de Hardware**

Los sensores se conectan al ESP32-Wroom según las siguientes especificaciones:

- **MQ135 (Analógico):**
  - **VCC:** 3.3V
  - **GND:** GND
  - **Salida de señal:** GPIO36 (ADC1_CH0)  
    
- **ML8511(Analógico):**
  - **VCC:** 3.3V
  - **GND:** GND
  - **Salida de señal:** GPIO33 (ADC5_CH0)
  
- **AHT25 (I2C):**
  - **VCC:** 3.3V
  - **GND:** GND
  - **SDA:** GPIO21
  - **SCL:** GPIO22
  
- **BMP280 (I2C):**
  - **VCC:** 3.3V
  - **GND:** GND
  - **SDA:** GPIO21
  - **SCL:** GPIO22  
  
- **BH1750 (I2C):**
  - **VCC:** 3.3V
  - **GND:** GND
  - **SDA:** GPIO21
  - **SCL:** GPIO22
  
- **Display LCD (I2C):**
  - **VCC:** 3.3V
  - **GND:** GND
  - **SDA:** GPIO21
  - **SCL:** GPIO22
  
- **Tira led WS2812:**
  - **VCC:** 3.3V
  - **GND:** GND
  - **CONTROL:** GIOP39
  
 

# Funcionamiento

Fórmulas para el Cálculo del AQI
El cálculo del AQI para cada contaminante se realiza mediante la siguiente fórmula general:  
  
  <center><img src="./assets/formula AQI.jpg"></center>  

Donde:

- I_high y I_low son los valores de AQI correspondientes a las concentraciones de contaminantes C_high y C_low.
- C es la concentración medida del contaminante.  

Con el MQ135, podríamos utilizar la concentración de CO₂ y otros gases para derivar una estimación del AQI. Por ejemplo, la concentración de CO₂ en ppm (partes por millón) se puede convertir en un valor aproximado de AQI según las categorías de la EPA. 

Este comportamiento se actualiza en cada ciclo del bucle principal.


## Que es un filtro de media movil?
Un filtro de media móvil es un método numérico utilizado para reducir el ruido y la volatilidad en una serie de datos, permitiendo visualizar tendencias y patrones más claros. Su función básica es calcular la media de los valores más recientes de la serie y utilizarla como una aproximación más estable de la tendencia actual.

En términos prácticos, un filtro de media móvil se aplica de la siguiente manera:

1. Se define un tamaño de ventana (N) que indica cuántos valores más recientes se consideran para calcular la media.
2. Se itera sobre la serie de datos, empezando desde el primer valor y hasta el valor actual.
3. Se calcula la media de los N valores más recientes, descartando el primer valor de la ventana al calcular la media y añadiendo el nuevo valor.
3. El resultado es el valor filtrado, que se utiliza como una estimación más estable de la tendencia actual.   

**Ventajas y desventajas**  
* Ventajas:   
  - sencillo de implementar  
  - Rápido de calcular   
  - Eficaz para reducir el ruido de alta frecuencia.  

* Desventajas:   
  - Puede eliminar componentes auténticos de la señal si el tamaño de la ventana es demasiado grande.   
  - Puede ser débil para detectar cambios bruscos en la tendencia.

### 3. **Compilación y Carga del Código**

Seguir los siguientes pasos:  

1. Abrir el proyecto en el IDE Platformio o en VScode con Platformio integrado.
2. Seleccionar la placa **nodemcu-32s** en el menú **Tools > Board**.
3. Seleccionar el puerto correcto donde está conectado el ESP32.
4. Compilar y cargar el código en el ESP32.

### 4. **Monitoreo de Datos**

Abrir el **Monitor Serie** en el IDE y configurarlo a **115200 baudios** para observar las lecturas de los sensores. Los datos se mostrarán en tiempo real y se indicará si alguna lectura es inválida debido a valores fuera de los rangos definidos.

## Futuras Expansiones

Este proyecto está diseñado para ser fácilmente escalable.   
Se puedes agregar más sensores o funcionalidades, como enviar los datos a una plataforma en la nube o a un servidor remoto.   
El código modular facilita la incorporación de nuevas características sin afectar la estructura existente.

## Licencia

Este proyecto está licenciado bajo la Licencia Creative Commons Atribución-NoComercial (CC BY-NC). Esta licencia permite que otros remixen, adapten y construyan sobre el trabajo de forma no comercial y, aunque sus nuevos trabajos deben también reconocer al autor original y ser no comerciales, no tienen que licenciar sus obras derivadas en los mismos términos.

Esta licencia es adecuada para un uso educativo y de aprendizaje, permitiendo la libre distribución y utilización del material mientras se protege contra el uso comercial sin autorización previa. Para usos comerciales, es necesario contactar a los autores para obtener permiso.

Para obtener más información sobre esta licencia y ver una copia completa de sus términos, visite [Creative Commons Atribución-NoComercial (CC BY-NC)](https://creativecommons.org/licenses/by-nc/4.0/).