#ifndef UserTypes_h
#define UserTypes_h
#include "Arduino.h"
#define FILE_BASE_NAME "cansat"

struct data_t
{
  unsigned long tim;
  double acc[3], gyro[3];
  float magn[3], temp, pres, alt;
};

typedef struct 
{
  uint32_t tempo;
  int16_t ac[3], gyr[3], mag[3];
  float temp, pres;
}packet;

byte userSetup();
void printHeader(Print* pr);
void acquireData(packet* raw);
void processData(packet raw, data_t* dadosConvertidos);
bool sendPacket(packet* pacote);
void printData(Print* pr, data_t* data);
void ledOut(char cor, bool state);

extern const int buttonPin;

#endif  // UserTypes_h
