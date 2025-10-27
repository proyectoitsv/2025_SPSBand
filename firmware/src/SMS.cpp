#include "SMS.h"

#define CONFIGSET "AT&K0;E0;V1;F1\r"
#define GETNUM "AT+CNUM\r"
#define TEXTMODE "AT+CMGF="
#define SEND "AT+CMGS="

smsSerial::smsSerial(short rx, short tx, int baud){
  uart = new SoftwareSerial(rx, tx);
  uart->begin(baud);
  uart->print(CONFIGSET);
  waitResponse();
}
smsSerial::~smsSerial(){
  delete uart;
}

void smsSerial::textMode(short toggle){
  char enable = (toggle = 0)?('0'):('1'); 
  uart->println(TEXTMODE);
  uart->write(enable);
  uart->write("\r");
  waitResponse(); 
}

String smsSerial::getNumber(){
  String rtrn = "";
  uart->println(GETNUM);
  while(uart->available()){
    rtrn+= uart->read();
  }
  return rtrn;
}

void smsSerial::sendMessage(String number, String message){
  uart->print(SEND);
  uart->print("\""+ number+ "\"\r");
  uart->print(message);
  uart->write(26);
  waitResponse(5000);
}

String smsSerial::waitResponse(const int timeout){
  unsigned long t0 = millis();
  String response = "";
  
  while(millis-t0<timeout){
    while(uart->available()){
      char c = uart->read();
      response+= c;
      if(response.endsWith("\r\nOK\r\n")||response.endsWith("\r\nERROR\r\n")){
        return response;
      } 
    }
    yield();
  }

  return response;
}