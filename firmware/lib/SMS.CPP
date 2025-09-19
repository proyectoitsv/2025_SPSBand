#include <SMS.h>

#define GETNUM "AT+CNUM"
#define TEXTMODE "AT+CMGF="
#define SEND "AT+CMGS="

smsSerial::smsSerial(short rx, short tx, int baud){
  uart = new SoftwareSerial(rx, tx);
  uart->begin(baud);
}
smsSerial::~smsSerial(){
  delete uart;
}

void smsSerial::textMode(short toggle){
  char enable = (toggle = 0)?('0'):('1'); 
  uart->println(TEXTMODE);
  uart->write(enable);
  uart->println(); 
}

String smsSerial::getNumber(){
  String rtrn = "";
  uart->println(GETNUM);
  while(uart->available()){
    String+= uart->read();
  }
  return rtrn;
}

void smsSerial::sendMessage(String number, String message){
  uart->print(SEND);
  uart->println(number);
  uart->print(message);
  uart->write(26);
}