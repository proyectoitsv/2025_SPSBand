// Codigo GPS google maps preciso
#include <Arduino.h>
#include <TinyGPSPlus.h>
#define led PIN_015

TinyGPSPlus gps;
#define RX_PIN PIN_008  
#define TX_PIN PIN_006  
String location = "https://google.com/maps/place/";

void setup() {
  Serial.begin(115200);        
  Serial1.begin(9600);
  Serial.println("Iniciando GPS...");
}

void loop() { 
  while (Serial1.available() > 0) {
    gps.encode(Serial1.read());

    if(gps.location.isUpdated()){
      location+= String(gps.location.lat(), 6);
      location+=", ";
      location+=String(gps.location.lng(), 6);
      Serial.println(location);
    }
    location = "https://google.com/maps/place/";

  }
  
}
