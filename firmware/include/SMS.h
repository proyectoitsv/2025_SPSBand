#ifndef SMSSERIAL_H
#define SMSSERIAL_H

#include <SoftwareSerial.h>

class smsSerial{
    private:
        SoftwareSerial *uart;
    public:
        smsSerial(short rx, short tx, int baud =9600);
        ~smsSerial();
        void textMode(short enable);
        void sendMessage(String number, String message);
        String getNumber();
}; 

#endif