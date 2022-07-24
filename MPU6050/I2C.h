#include <stdint.h>
#include <stdbool.h>

#include "..\Wire\Wire.h"
#include "..\timer-driver.h"

void selectAdress(uint8_t adress);

uint8_t i2cWrite(uint8_t registerAddress, uint8_t data, bool sendStop) ;

uint8_t i2cWrite(uint8_t registerAddress, uint8_t* data, uint8_t length, bool sendStop);

uint8_t i2cRead(uint8_t registerAddress, uint8_t* data, uint8_t nbytes);
