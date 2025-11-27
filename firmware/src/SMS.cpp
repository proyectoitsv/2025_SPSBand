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
  uart->print("AT+CMGF=0");
  waitResponse();
}
smsSerial::~smsSerial(){
  delete uart;
}

void smsSerial::textMode(short toggle){
  char enable = (toggle = 0)?('0'):('1'); 
  uart->println(TEXTMODE);
  uart->write(enable);
  uart->write('\r');
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
  waitResponse(3000, ">");
  uart->print(message);
  uart->write(26);
  waitResponse(10000);
}

String smsSerial::waitResponse(const unsigned long timeout, const String wanted){
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