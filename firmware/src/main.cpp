#include "Oximetro.h"
#include "SMS.h"
#include "GPS.h"

#define RX_D PIN_006
#define TX_D PIN_008
#define RX_2 PIN_010
#define TX_2 PIN_011
gpsSerial mySerial(RX_2, TX_2);

String url = mySerial.getUrl();ú


void setup()
{
  Serial.begin(115200);
  Serial.println("Initializing...");
  
  if(i2cBegin(20, 0) != 0){
    Serial.println("Error");
    while(1);
  }
  Serial.println("I2C detected");
}

void loop(){  
  Serial.println("Hold the device in your wrist/finger for 10 seconds. May take some time to calibrate");
  Serial.println(getBeatAvg(10));

}

