#include "SMS.h"
#include "Oximetro.h"

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

