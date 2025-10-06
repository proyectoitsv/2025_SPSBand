//MAX30102 andando mas preciso 


#include "Oximetro.h"





MAX30105 particleSensor;

// Variables para muestreo cada 0,5 segundos
unsigned long lastSampleTime = 0;
const unsigned long sampleInterval = 500; // 500 ms

int i2cBegin(uint8_t redAmp, uint8_t greenAmp)
{
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST))
  {
    return -1;
  }

  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(redAmp);
  particleSensor.setPulseAmplitudeGreen(greenAmp);
  return 0;
}

int getBeatAvg(const int rateSize)
{
  const byte RATE_SIZE = rateSize; //Cantidad de valores para promediar
  byte rates[RATE_SIZE];
  byte rateSpot = 0;
  long lastBeat = 0;

  float beatsPerMinute;
  int beatAvg;
  long t0 = millis();
  int rateNum = 0;
  while(millis()-t0 < 10000){  
    long irValue = particleSensor.getIR();
    if (checkForBeat(irValue) == true)
    {
      long delta = millis() - lastBeat;
      lastBeat = millis();

      beatsPerMinute = 60 / (delta / 1000.0);

      rateNum++;
    }
  }
  beatAvg= rateNum*6;
  return beatAvg;
}