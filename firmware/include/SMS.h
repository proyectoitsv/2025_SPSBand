#ifndef SMSSERIAL_H
#define SMSSERIAL_H

#include <SoftwareSerial.h>
#include "Arduino.h"

class smsSerial{
    private:
        SoftwareSerial *uart;
    public:
        smsSerial(short rx, short tx, int baud =9600);
        ~smsSerial();
        void textMode(short enable);
        void sendMessage(String number, String message);
        String getNumber();
        String waitResponse(const unsigned long timeout = 3000, const String wanted = "OK\r\n");
}; 

#endif