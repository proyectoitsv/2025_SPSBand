#include "GPS.h"

gpsSerial::gpsSerial(short Rx, short Tx){
    uart = new SoftwareSerial(Rx, Tx);
    uart->begin(9600);
}

gpsSerial::~gpsSerial(){
    delete uart;
}

String gpsSerial::getUrl(){
    String location;
    while(uart->available()){
        gps.encode(uart->read());
    }
    location= "https://google.com/maps/place/" + String(gps.location.lat(), 6) +","+ String(gps.location.lng(), 6);
    return location;
}
