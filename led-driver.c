#include <stdint.h>

#include "led-driver.h"

uint8_t led_config(uint8_t led_num, uint8_t pin)
{
	uint8_t sucess=0;
	if (led_num < MAX_LEDS)
	{
	  pin_map[MAX_NUM-led_num-1]=pin;

	  struct bit_and_port num_bit_porta = getPort(pin_map[MAX_NUM-led_num-1]);
	  	  
	  if(port_init[num_bit_porta.indice_porta]!=1) 
		{ /* Inicializa porta apenas se ela ainda não foi inicializada */
			gpio_init(num_bit_porta.dev_ptr);
			port_init[num_bit_porta.indice_porta]=1;
		}
		  
	  if(num_bit_porta.dev_ptr==GPIOC)
	  { /* Pinos da porta GPIOC só podem receber corrente (só open drain) */
		gpio_set_mode(num_bit_porta.dev_ptr, num_bit_porta.bit, GPIO_OUTPUT_OD);
	  }	
	  else
	  {
		gpio_set_mode(num_bit_porta.dev_ptr, num_bit_porta.bit, GPIO_OUTPUT_PP);
	  }
	  sucess=1;
	}
	else
	{
	  /*"LED out of bounds!"*/
	}
	return sucess;
}

uint8_t  led_on(uint8_t led_num) //Ligar
{
	uint8_t sucess=0;
	struct bit_and_port num_bit_porta= getPort(pin_map[MAX_NUM-led_num-1]);
	if (led_num < MAX_LEDS)
	{	
	  if(num_bit_porta.dev_ptr==GPIOC)
	  { /* Pinos da porta GPIOC  são open drain, logo é invertido */
		gpio_write(MAX_NUM-led_num-1, 0);
	  }	
	  else
	  {
		gpio_write(MAX_NUM-led_num-1, 1);
	  }
	  sucess=1;
	}
	else
	{
	  /*"LED out of bounds!"*/
	}
	return sucess;
}

uint8_t  led_off(uint8_t led_num) //Desligar
{
	uint8_t sucess=0;
	struct bit_and_port num_bit_porta= getPort(pin_map[MAX_NUM-led_num-1]);
	if (led_num < MAX_LEDS)
	{
	  if(num_bit_porta.dev_ptr==GPIOC)
	  { /* Pinos da porta GPIOC  são open drain, logo é invertido */
		gpio_write(MAX_NUM-led_num-1, 1);
	  }	
	  else
	  {
		  gpio_write(MAX_NUM-led_num-1, 0);
	  }
		sucess=1;
	}
	else
	{
	  /*"LED out of bounds!"*/
	}
	return sucess;
}

uint8_t  led_toggle(uint8_t led_num) //Alternar
{
	uint8_t sucess=0;
	if (led_num < MAX_LEDS)
	{
		gpio_toggle(MAX_NUM-led_num-1);
		sucess=1;
	}
	else
	{
	  /*"LED out of bounds!"*/
	}
	return sucess;
}
