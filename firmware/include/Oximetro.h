#ifndef OXIMETRO_H
#define OXIMETRO_H

#include <Wire.h>
#include <Arduino.h>
#include "MAX30105.h"
#include "heartRate.h"

int i2cBegin(uint8_t redAmp, uint8_t greenAmp); //puede devolver error
int getBeatAvg(const int rateSize);


#endif