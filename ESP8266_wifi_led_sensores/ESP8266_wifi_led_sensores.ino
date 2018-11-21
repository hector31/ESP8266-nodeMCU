// This example shows how to connect to Cayenne using an ESP8266 and send/receive sample data.
// Make sure you install the ESP8266 Board Package via the Arduino IDE Board Manager and select the correct ESP8266 board before compiling. 

//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>

// WiFi network info.
char ssid[] = "Acosta aza";
char wifiPassword[] = "27104334";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "f6468fe0-eba9-11e8-8e60-53d8c8eabc71";
char password[] = "5c6cee12abda3970d58f33ee22dfe64a01a01cb0";
char clientID[] = "e1236100-ebaa-11e8-809d-0f8fe4c30267";

unsigned long lastMillis = 0;
//manejar datos desde cayenne
const int Led_01=14;
const int LedPWM=12;
//----variables a sensar
int ValorADC=0;
int MyTemperatura=0;
float PorcentajeCo2=14.56;
//---------------------
void setup() {
  //definir datos a controlar desde cayenne
  pinMode(Led_01,OUTPUT);
  pinMode(LedPWM,OUTPUT);
  //-------------------
	Serial.begin(9600);
	Cayenne.begin(username, password, clientID, ssid, wifiPassword);
}

void loop() {
	Cayenne.loop();

	//Publish data every 10 seconds (10000 milliseconds). Change this value to publish at a different interval.
	if (millis() - lastMillis > 10000) {
		lastMillis = millis();
		//Write data to Cayenne here. This example just sends the current uptime in milliseconds.
		
		//---------variables enviadas a cayenne
   ValorADC=analogRead(A0);
   MyTemperatura=map(ValorADC,0,1023,0,100);

   //como escribir en cayenne
   Cayenne.virtualWrite(0,MyTemperatura,TYPE_TEMPERATURE,UNIT_CELSIUS);
   Cayenne.virtualWrite(1,ValorADC);
   Cayenne.virtualWrite(2,PorcentajeCo2);

   PorcentajeCo2=PorcentajeCo2+4.72;
   if(PorcentajeCo2>100){PorcentajeCo2=0;}

   //aqui termina
		
		//Cayenne.virtualWrite(0, lastMillis);
		//Some examples of other functions you can use to send data.
		//Cayenne.celsiusWrite(1, 22.0);
		//Cayenne.luxWrite(2, 700);
		//Cayenne.virtualWrite(3, 50, TYPE_PROXIMITY, UNIT_CENTIMETER);
	}
}
//como ya he usado los canales 0,1,2 para sensar en lo siguiente se defienen canales 3,4 para controlar desde cayenne
CAYENNE_IN(3)
{
  int Valor_Canal_03=getValue.asInt();
  digitalWrite(Led_01,Valor_Canal_03);
}

CAYENNE_IN(4)
{
  int MySlider=getValue.asInt();
  analogWrite(LedPWM,MySlider);
}

//Default function for processing actuator commands from the Cayenne Dashboard.
//You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
	CAYENNE_LOG("CAYENNE_IN_DEFAULT(%u) - %s, %s", request.channel, getValue.getId(), getValue.asString());
	//Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}
