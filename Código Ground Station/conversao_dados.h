#include <Arduino.h>
#include "UserTypes.h"

extern uint32_t startMicros;

void processData(packet raw, data_t* dadosConvertidos);
void printHeader(Print * pr);
void printData(Print * pr, data_t* dataSD); //Print a data record.
