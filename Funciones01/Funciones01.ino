/*
Sensor de temperatura y humedad
Boton1: Manual. Activa la refrigeración al ser presionado
Boton2: Alta demanda
Boton3: Sobre carga de funcionamiento
Led1: Refrigeracion manual
Led2: Refrigeración automática
1. Que se encienda la refrigeracion manual cada que se presione el boton de manual
2. La refrigeracion manual tiene prioridad a la refrigeracion automática.
3. Si la termperatura es alta (mayor a 28, por ejemplo), se activa la refrigeracion automática
4. Si tengo alta demanda o sobre carga, se activa la refrigeración automática
5. Si tengo alta demanda o sobrecarga de funcionamiento y ademas temperatura alta, se activan ambas refrigeraciones
Comportamiento
Entradas    B1, B2, B3, Temp (0 cuando temp <= 28, 1 cuando temp > 28)
Salidas     Led1, Led2
B1  B2  B3  Temp  Led1  Led2
0   0   0   0     0     0
0   0   0   1     0     1
0   0   1   0     0     1
0   0   1   1     1     1
0   1   0   0     0     1
0   1   0   1     1     1
0   1   1   0     0     1
0   1   1   1     1     1
1   0   0   0     1     0
1   0   0   1     1     1
1   0   1   0     1     1
1   0   1   1     1     1
1   1   0   0     1     1
1   1   0   1     1     1
1   1   1   0     1     1
1   1   1   1     1     1
Led1 = B3Temp + B2Temp + B1
Led2 = Temp + B3 + B2
Botones 14, 15, 13
Leds 4, 2
DHT11 12
*/
#include "DHT.h"

#define DHTPIN 15     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
int estadoBoton1 = 0, estadoBoton2 = 0, estadoBoton3 = 0;
bool led1 = false, led2 = false, temBool = false;
long timeNow, timeLast; // Variables de control de tiempo no bloqueante
int wait = 2000;// Indica la espera cada 2 segundos para envío de mensajes MQTT


void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");

  dht.begin();
  pinMode(13, INPUT);
  pinMode(14, INPUT);
  pinMode(15, INPUT);
  
  pinMode(4, OUTPUT);
  pinMode(2, OUTPUT);  
  timeLast = millis (); // Inicia el control de tiempo
}

void loop() {
  //lectura de botones
  estadoBoton1 = digitalRead(13);
  estadoBoton2 = digitalRead(14);
  estadoBoton3 = digitalRead(15);

  float t = dht.readTemperature();
  if(t <= 28)
    temBool = false;
  else if(t > 28)
    temBool = true;

  //logica
  //B3Temp + B2Temp + B1
  led1 = estadoBoton3 & temBool | estadoBoton2 & temBool | estadoBoton1;

  //Temp + B3 + B2
  led2 = temBool | estadoBoton3 | estadoBoton2;
  timeNow = millis();
  if (timeNow - timeLast > wait){
    timeLast = timeNow;
    if(led1 == true){
      digitalWrite(2, HIGH);
    }else{
      digitalWrite(2, LOW);
    }

    if(led2 == true){
      digitalWrite(4, HIGH);
    }else{
      digitalWrite(4, LOW);
    }
  }
}
