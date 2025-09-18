#include <Arduino.h>
#include <SoftwareSerial.h>
#define RX_D PIN_006
#define TX_D PIN_008
#define PING "AT+CNUM"
#define MODO_TEXTO "AT+CMGF=1"
#define AGENDAR_NUM "AT+CMGS=\"+5493515554940\""

SoftwareSerial mySerial1(RX_D, TX_D);

void setup(){
    Serial.begin(115200);
    mySerial1.begin(19200);
    delay(3000);
    //mySerial1.println(MODO_TEXTO);

}

void loop(){  
  mySerial1.println(PING);
    mySerial1.println(MODO_TEXTO);
    delay(500);
    mySerial1.println(AGENDAR_NUM);
    mySerial1.print("Mensaje desde el módem SIM800L");
    delay(2000);
    mySerial1.write(26);
    
    delay(500);
    while (mySerial1.available()) { //Imprimir cada caracter recibido en el micro
    char c = mySerial1.read();
    Serial.print(c);
  }
  delay(20000);

}