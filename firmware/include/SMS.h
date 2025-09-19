#ifndef SMSSERIAL_H
#define SMSSERIAL_H

#include <SoftwareSerial.h>

class smsSerial{
    private:
        SoftwareSerial* uart;
    public:
        smsSerial(short rx, short tx, int baud);
        ~smsSerial();
        void textMode(short toggle);
        String getNumber();
        void sendMessage(String number, String message);
        
}; 

#endif