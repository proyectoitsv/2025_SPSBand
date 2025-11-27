#include "GPS.h"

gpsSerial::gpsSerial(short Rx, short Tx){
    uart = new SoftwareSerial(Rx, Tx);
}

gpsSerial::~gpsSerial(){
    delete uart;
}

String gpsSerial::getUrl(bool en){
    if(en == 0){
        uart->end();
        return "N/A";
    }
    uart->begin(9600);
    unsigned long t0 = millis();
    String location = "https://www.google.com/maps/search/?api=1&query=";
    while(millis()-t0<3000){
        while(uart->available()){
            gps.encode(uart->read());
        }
    }
    if(gps.location.isValid()){
        double lat = gps.location.lat();
        double lon = gps.location.lng();
        location+= String(lat, 6)+","+String(lon, 6);
        return location;
    }
    else{
        return location+"0.000000,0.000000";
    }
}
