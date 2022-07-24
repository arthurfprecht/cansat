#include <stdint.h>
#include <stdbool.h>

#include "../Wire/Wire.h"
#include "I2C.h"

#include "..\usart-driver.h"


void inicializaMPU(int MPU);

bool recebeDados(int MPU,  int16_t acc[], int16_t gyro[]);

void converteDados(const int16_t acc[], const int16_t gyro[], 
				   double acc_conv[], double gyro_conv[] );

void testaMPU(uint8_t MPU_addr);
