#include <Arduino.h>
#include <SoftwareSerial.h>
#include <NRF52TimerInterrupt.h>
#include "GPS.h"
#include "Oximetro.h"
/*
AHGORA

*/ 

//Configuración de Timer
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     3
#define TIMER0_INTERVAL_MS            1000
#define TIMER0_DURATION_MS            5000

//Pines de los módulos
#define TX_D PIN_006
#define RX_D PIN_008
#define RX_2 PIN_029
#define TX_2 PIN_031
#define PB_U PIN_020

//Comandos AT
#define PING            "AT+CNUM\r"
#define MODEM_SETUP     "AT&K0;E1;V1\r"
#define MODO_TEXTO      "AT+CMGF=1\r"
#define MODO_TEXTO_OFF  "AT+CMGF=0\r"
#define AGENDAR_NUM     "AT+CMGS=\"+5493515554940\"\r"

SoftwareSerial mySerial1(RX_D, TX_D);
String masage = "";
volatile uint32_t preMillisTimer0 = 0;
bool toggle0 = 0;
void PBInterrupt();
int randomBeatGenerator(int state);
volatile bool PBUIF = 0;   //flag del pulsador (Push Button (of the) User Interrupt Flag)

NRF52Timer ITimer0(NRF_TIMER_1);
volatile int count =0;

void timerHandler();

String readResponse(unsigned long timeout = 3000, String wanted = "OK\r\n");
gpsSerial *serial2;

void setup()
{
  // Serial.begin(115200);
  mySerial1.begin(115200);
  serial2 = new gpsSerial(RX_2, TX_2);
  delay(3000);
  // i2cBegin(0x0A,0x00);
  mySerial1.print(MODO_TEXTO_OFF);
  if(readResponse().indexOf(">")!=-1){
    mySerial1.print(0x1A);
    readResponse(9000);
    mySerial1.print(MODO_TEXTO_OFF);
    readResponse();
  }

  pinMode(PB_U, INPUT_PULLDOWN);

  // Serial.println("Place your index finger on the sensor with steady pressure.");
  mySerial1.print(MODEM_SETUP);
  readResponse();
  mySerial1.print(MODO_TEXTO);
  readResponse();
  
  
  // for(int i = 0; i<12; i++)
  //   msg+= mySerial1.read();
  // if(msg.endsWith("READY")){
  //   mySerial1.println(MODO_TEXTO);
  // }
  // else{
  //   Serial.println("SIM800L no esta listo");
  // }
  
  ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, timerHandler);
  attachInterrupt(digitalPinToInterrupt(PB_U), PBInterrupt, RISING);
  toggle0= 1;

}

void loop(){  
  int beatss = randomBeatGenerator(rand() % 3);
  masage = serial2->getUrl(1)+" "+(String)beatss + " BPM";
  
  if(PBUIF == 1 ){
    masage+= " ALERTA DE PANICO ";
  }
  if(beatss<60){
    masage+= " ALERTA BRADICARDIA ";
  }
  if(beatss>120){
    masage+= " ALERTA TAQUICARDIA ";
  }

  mySerial1.print("AT+CSQ?\r");
  if(readResponse().indexOf("99,99") == -1){
    mySerial1.print("AT+CREG?\r");
    String resp =readResponse();
    if(masage.indexOf("ALERTA") != -1){
      mySerial1.print(AGENDAR_NUM);
      readResponse(3000, ">");
      mySerial1.print(masage);
      mySerial1.write(0x1A);
      readResponse(10000);
    }
    else if(resp.indexOf(",1") == -1){
      mySerial1.print("DEBUG: SIN REGISTRO EN RED\r");
    }
  }
  else{
    mySerial1.print("DEBUG: SIN SEÑAL\r");
  }
  // mySerial1.print(MODO_TEXTO_OFF);
  // readResponse();
  // while(true){
  //   if(mySerial1.available()!=0){
  //     Serial.println("Myserial1 available");
  //     break;
  //   }
  //   else if(millis()-ti0 > 3000){
  //     Serial.println("Myserial1 not available");
  //     break;
  //   }
  // }
  // while (mySerial1.available()) { //Imprimir cada caracter recibido en el micro
  //   char c = mySerial1.read();
  //   Serial.print(c);
  // }
  delay(100);

}

String readResponse(unsigned long timeout, String wanted) {
  String resp = "";
  unsigned long start = millis();

  while (millis() - start < timeout) {
    if (mySerial1.available()) {           
      char c = mySerial1.read();           
      resp += c;                           
      if(resp.endsWith(wanted) || resp.endsWith("ERROR\r\n")) break; 
    }
  }

  return resp;
}

void timerHandler(){
  count++;
  if(count>=5){
    toggle0= (toggle0==0)?1:0;
    count = 0; 
  }
  
}

// void PBInterrupt(){
//   bool pbState = digitalRead(PB_U);
//   unsigned long pbStart = 0;
//   if(pbState == 0){
//     if(pbStart == 0){
//       pbStart = millis();
//     }
//     if(millis()-pbStart >=3000){
//       if(!PBUIF){
//         PBUIF = 0;
//       }
//     }
//   }
//   else{
//     if(pbStart !=0){
//       pbStart = 0;
//     }
//   }
// }
void PBInterrupt() {
  static unsigned long lastInterrupt = 0;
  unsigned long now = millis();

  // Debounce de 150 ms para evitar toggles repetidos por rebote
  if (now - lastInterrupt > 150) {
    PBUIF = !PBUIF;   // toggle
  }

  lastInterrupt = now;
}




int randomBeatGenerator(int state){
  if (state == 0)
    return(rand()%(120-60+1)+60);
  else if (state == 1)
    return(rand()%(60-20+1)+20);
  else if (state == 2)
    return(rand()%(180-120+1)+120);
}