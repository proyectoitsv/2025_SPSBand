#include "Oximetro.h"
#include "SMS.h"
#include "GPS.h"

#define RX_D PIN_006
#define TX_D PIN_008
#define RX_2 PIN_010
#define TX_2 PIN_011
#define NUMERO "+549351xxxxxxx" //Cambiar por mi número de teléfono

gpsSerial *SerialGPS;
smsSerial *SerialSMS;

String url = SerialGPS.getUrl();
long time0 =0;

void setup()
{
  Serial.begin(115200);
  Serial.println("Initializing...");
  SerialGPS = new gpsSerial(RX_2, TX_2);
  SerialSMS = new smsSerial(RX_D, TX_D);
  if(i2cBegin(20, 0) != 0){
    Serial.println("Error");
    while(1);
  }
  Serial.println("I2C detected");
  time0 = millis();
}

void loop(){  
  Serial.println("Mantené el dispositivo en tu muñeca o dedo durante al menos 10 segundos.");
  int beatAvg = getBeatAvg(10);
  time0 = time;
  String location = SerialGPS.getUrl();
  String message = "Ritmo cardíaco: "+ (String)beatAvg+" BPM "+ "Ubicación: " + location;
  SerialSMS->textMode(1);
  SerialSMS->sendMessage(NUMERO, message);
  SerialSMS->textMode(0);
  Serial.println("Mensaje enviado: \""+ message + "\"");
}

