#include <Arduino.h>
#include <MAX30105.h>
#include <GPS.h>
#include <heartRate.h>
#include <Wire.h>
#define RX_1 PIN_006
#define TX_1 PIN_008
#define RX_2 PIN_010
#define TX_2 PIN_011
#define PING "AT+CNUM"
#define MODO_TEXTO "AT+CMGF=1"
#define AGENDAR_NUM "AT+CMGS=\"+5493515554940\""

int promediarLatidos(MAX30105 obj);

gpsSerial mySerial(RX_2, TX_2);

String url = mySerial.getUrl();ú

SoftwareSerial mySerial1(RX_D, TX_D);

MAX30105 particleSensor;

const byte RATE_SIZE = 4; //Cantidad de valores para promediar
byte rates[RATE_SIZE];
byte rateSpot = 0;
long lastBeat = 0;

float beatsPerMinute;
int beatAvg;

// Variables para muestreo cada 0,5 segundos
unsigned long lastSampleTime = 0;
const unsigned long sampleInterval = 500; // 500 ms

void setup()
{
  Serial.begin(115200);
  Serial.println("Initializing...");
  mySerial1.begin(19200);

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST))
  {
    Serial.println("MAX30102 was not found. Please check wiring/power.");
    while (1);
  }
  
  Serial.println("Place your index finger on the sensor with steady pressure.");

  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x0A);
  particleSensor.setPulseAmplitudeGreen(0);
  
  String msg = "";
  for(int i = 0; i<12; i++)
    msg+= mySerial1.read();
  if(msg.endsWith("READY")){
    mySerial1.println(MODO_TEXTO);
  }
  else{
    Serial.println("SIM800L no esta listo");
  }

}

void loop(){  
  int promedioBPM = promediarLatidos(particleSensor);

  delay(500);
  mySerial1.println(AGENDAR_NUM);
  mySerial1.print("INFORMACIÓN EN TIEMPO REAL DEL USUARIO ");
  mySerial1.print("Ritmo Cardíaco: ");
  mySerial1.print(promedioBPM);
  mySerial1.print(" BPM"); 
  delay(2000);
  mySerial1.write(26);
    
  delay(500);
  while (mySerial1.available()) { //Imprimir cada caracter recibido en el micro
    char c = mySerial1.read();
    Serial.print(c);
  }
  delay(3000);

}

int promediarLatidos(MAX30105 obj)
{
  long irValue = obj.getIR();

  if (checkForBeat(irValue) == true)
  {
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute;
      rateSpot %= RATE_SIZE;

      beatAvg = 0;
      for (byte x = 0; x < RATE_SIZE; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }
  return beatAvg;
}
