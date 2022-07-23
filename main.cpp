#include <stdint.h>
#include <stdbool.h>

#include "Core\sysInit.h"

#include "gpio-driver.h"
#include "led-driver.h"
#include "timer-driver.h"
#include "pwm-driver.h"
#include "adc-driver.h"
#include "usart-driver.h"

int main()
{
  	systemInit();
  	uart_config(1, 57600);
	printf("Iniciando teste de millis: \n");
	while(!buffer_empty(1, 'w'));
	
  	start_millis(2); 
	while(1)
	{
	  printf("Tempo =  %i ms \n", time_miliseconds);
	  while(!buffer_empty(1, 'w'));
	}
  	return 0;
}
