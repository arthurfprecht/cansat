#include <stdint.h>
#include <stdbool.h>
#include "gpio-driver.h"

uint8_t pin_map[MAX_NUM]; 	/* Vetor para mapear os pinos e os números 
						de GPIO atribuídos. Os números vão de 0 a MAX_NUM-1*/
						  
bool port_init[] = {0,0,0}; 

uint8_t  gpio_config(uint8_t gpio_num, uint8_t pin, gpio_pin_mode dir)
{
  	uint8_t sucess=0;
	  if (gpio_num<MAX_NUM)
	  {
		pin_map[gpio_num]=pin;

		struct bit_and_port num_bit_porta= getPort(pin_map[gpio_num]);
				
		if(port_init[num_bit_porta.indice_porta]!=1) 
		{ /* Inicializa porta apenas se ela ainda não foi inicializada */
			gpio_init(num_bit_porta.dev_ptr);
			port_init[num_bit_porta.indice_porta]=1;
		}
		if((num_bit_porta.dev_ptr!=GPIOC) || (dir!=GPIO_OUTPUT_PP))
		{ /* Pinos da porta GPIOC só podem receber corrente (só open drain) */
			gpio_set_mode(num_bit_porta.dev_ptr, num_bit_porta.bit, dir);
			sucess=1;
		}
		else
		{
		  /*"Prohibited output type for this port!"*/
		}
	  }
	  else
	  {
		/*"Pin out of bounds!"*/
	  }
	return sucess;
}

uint8_t gpio_write(uint8_t gpio_num, uint8_t val) //Escrever sinal digital no pino
{
    uint8_t sucess=0;
	if (gpio_num<MAX_NUM)
	{
		struct bit_and_port num_bit_porta= getPort(pin_map[gpio_num]);
	
		gpio_write_bit(num_bit_porta.dev_ptr, num_bit_porta.bit, val);
		  
		sucess=1;
	}
	else
	{
	  /* "Pin out of bounds!"*/
  	}
	return sucess;
}

uint8_t gpio_toggle(uint8_t gpio_num) //Alternar o sinal escrito no pino
{
    uint8_t sucess=0;
	if (gpio_num<MAX_NUM)
	{
	  struct bit_and_port num_bit_porta= getPort(pin_map[gpio_num]);
	  
	  gpio_toggle_bit(num_bit_porta.dev_ptr, num_bit_porta.bit);
	}
	else
	{
	  /*"Pin out of bounds!"*/
	}
	return sucess;
}

gpio_pin_mode gpio_dir(uint8_t gpio_num) //Lê qual a função que o pino selecionado tem 	
{
  	gpio_pin_mode mode = GPIO_INPUT_ANALOG;
	if (gpio_num<MAX_NUM)
	{
		struct bit_and_port num_bit_porta= getPort(pin_map[gpio_num]);
	  
		mode = gpio_get_mode(num_bit_porta.dev_ptr, num_bit_porta.bit);
	}
	else
	{
	  /* "Pin out of bounds!"*/
	}
	return mode;
}

uint8_t gpio_read(uint8_t gpio_num, uint8_t *val) //Ler sinal digital no pino
{
    uint8_t sucess=0;
	if (gpio_num<MAX_NUM)
	{
		struct bit_and_port num_bit_porta= getPort(pin_map[gpio_num]);

		*val = gpio_read_bit(num_bit_porta.dev_ptr, num_bit_porta.bit) ? 1 : 0;
	}
	else
	{
	  /*"Pin out of bounds!"*/
  	}
	return sucess;
}

/* Funções auxiliares da identificação da porta do microcontrolador */

struct bit_and_port getPort(uint8_t pin)
{
  struct bit_and_port num_bit_porta = {NULL, 0, 0}; 
	if (pin>PB15)
	{
		num_bit_porta.dev_ptr = GPIOC;
		num_bit_porta.bit = pin - 32;
		num_bit_porta.indice_porta = 2;
	}	
	else if(pin>PA15)
	{
		num_bit_porta.dev_ptr = GPIOB;
		num_bit_porta.bit = pin - 16;
		num_bit_porta.indice_porta=1;
	}
	else
	{
		num_bit_porta.dev_ptr = GPIOA;
		num_bit_porta.bit = pin - 0;
		num_bit_porta.indice_porta = 0;
	}
	return num_bit_porta;
}
