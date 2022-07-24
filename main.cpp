#include <stdint.h>
#include <stdbool.h>

#include "Core\sysInit.h"

#include "gpio-driver.h"
#include "led-driver.h"
#include "timer-driver.h"
#include "pwm-driver.h"
#include "adc-driver.h"
#include "usart-driver.h"

#include "SPI/SPI.h"
#include "Wire/Wire.h"
#include "MPU6050/MPU6050.h"
#include "BMP280/BMP280-library.h"

bmp280 bmp_280;

int main()
{
  	systemInit();
	start_millis(2); 
  	uart_config(1, 57600);
	
	printf("Iniciando teste de I2C: \n");
	while(!buffer_empty(1, 'w'));
	
	bmp_280.begin();
	inicializaMPU(0x68);
	
  	while(1)
	{

	  double temperatura, pressao, altitude;
	  temperatura = bmp_280.readTemperature();
  	  pressao = bmp_280.readPressure()*1000;
  	  altitude = bmp_280.calcAltitude(pressao,1021000); // this should be adjusted to your local forcase
	  
	  printf("Temperatura: %.1foC \tPressao: %.3f kPa \tAltitude: %.2fm \n",
			 temperatura, pressao/1000, altitude);
	  while(!buffer_empty(1, 'w'));
	  testaMPU(0x68);
	}
  	return 0;
}
