#include <stdint.h>
#include <stdbool.h>
#include "Core/gpio.h"

#ifndef gpio_driver_h
#define gpio_driver_h

struct bit_and_port
{
  gpio_dev* dev_ptr;
  uint8_t bit, indice_porta;
	
};

enum {PA0=0, PA1=1, PA2=2, PA3=3, PA4=4, PA5=5, PA6=6, PA7=7, PA8=8, PA9=9, 
		PA10=10, PA11=11, PA12=12, PA15=15,
	  PB0=16, PB1=17, PB3=19, PB4=20, PB5=21, PB6=22, PB7=23, PB8=24,
		PB9=25, PB10=26, PB11=27, PB12=28, PB13=29, PB14=30, PB15=31,
	  PC13=45,	PC14=46, PC15=47};
		
#define MAX_NUM 47 /* sizeof(pinos_disponiveis)/sizeof(uint8_t) */
#define HIGH 1
#define LOW 0

extern uint8_t pin_map[];
extern bool port_init[];

struct bit_and_port getPort(uint8_t pin);

uint8_t gpio_config(uint8_t gpio_num, uint8_t pin, gpio_pin_mode dir);

uint8_t gpio_write(uint8_t gpio_num, uint8_t val); //Escrever sinal digital no pino

uint8_t gpio_toggle(uint8_t gpio_num); //Alternar o sinal escrito no pino

gpio_pin_mode gpio_dir(uint8_t gpio_num); //Muda o modo do pino de entrada para saída ou vice versa 	

uint8_t gpio_read(uint8_t gpio_num, uint8_t *val); //Ler sinal digital no pino

#endif
