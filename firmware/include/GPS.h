// Codigo GPS google maps preciso
#ifndef GPSSERIAL_H
#define GPSSERIAL_H

#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

class gpsSerial{
  private:
    SoftwareSerial *uart;
    TinyGPSPlus gps;
  public:
    gpsSerial(short Rx, short Tx);
    ~gpsSerial();
    String getUrl(bool en);
};

#endif