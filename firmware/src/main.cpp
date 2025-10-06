#include "Oximetro.h"
#include "SMS.h"
#include "GPS.h"

#define RX_D PIN_006
#define TX_D PIN_008
#define RX_2 PIN_010
#define TX_2 PIN_011
gpsSerial mySerial(RX_2, TX_2);

String url = mySerial.getUrl();ú

#define NUMERO "+5493515554940"
smsSerial *mySerial = nullptr;

void setup()
{
  Serial.begin(115200);
  mySerial = new(smsSerial(PIN_006, PIN_008, 19200));
  Serial.println("Initializing...");
  
  if(i2cBegin(20, 0) != 0){
    Serial.println("Error");
    while(1);
  }
  Serial.println("I2C detected");
}

void loop(){  
  long tStart = 0;
  tStart = (tStart==0)?millis():tStart;
  Serial.println("Hold the device in your wrist/finger for 10 seconds. It may take some time to calibrate");
  int beatAvg = getBeatAvg(4);
  Serial.println(beatAvg);
  if(millis()-tStart > 10000){
    mySerial->textMode(1);
    mySerial->sendMessage(NUMERO, (String)beatAvg);
    mySerial->textMode(0);
    tStart = 0;
  }

}

