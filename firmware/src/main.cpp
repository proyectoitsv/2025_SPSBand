#include <SMS.h>
#include <Oximetro.h>

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

