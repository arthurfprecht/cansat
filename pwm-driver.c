#include <stdint.h>
#include <stdbool.h>

#include "Core\libmaple_types.h"
#include "Core\gpio.h"
#include "Core\timer.h"

#include "gpio-driver.h"
#include "pwm-driver.h"

#define PWM_MAX_NUM 12

static uint8_t pwm_pin_map[PWM_MAX_NUM];
uint8_t get_timer_num(timer_dev *dev);

const stm32_pin_info PIN_MAP[] = {

    {&gpioa, &timer2,  0, 1}, /* PA0 */
    {&gpioa, &timer2,  1, 2}, /* PA1 */
	{&gpioa, &timer2,   2, 3}, /* PA2 */	
    {&gpioa, &timer2,  3, 4}, /* PA3 */
    {&gpioa,   NULL,   4, 0}, /* PA4 */
    {&gpioa,   NULL,   5, 0}, /* PA5 */
    {&gpioa, &timer3,   6, 1}, /* PA6 */
    {&gpioa, &timer3,   7, 2}, /* PA7 */
    {&gpioa, &timer1,   8, 1}, /* PA8 */	
	{&gpioa, &timer1,   9, 2}, /* PA9 */	
    {&gpioa, &timer1,  10, 3}, /* PA10 */
    {&gpioa, &timer1,  11, 4}, /* PA11 */
    {&gpioa,   NULL,  12, 0}, /* PA12 */	
    {&gpioa,   NULL,  13, 0}, /* PA13 */	
    {&gpioa,   NULL,  14, 0}, /* PA14 */
    {&gpioa,   NULL,  15, 0}, /* PA15 */
	
    {&gpiob, &timer3,   0, 3}, /* PB0 */	
    {&gpiob, &timer3,   1, 4}, /* PB1 */
    {&gpiob,   NULL,   2, 0}, /* PB2 */
    {&gpiob,   NULL,  3, 0}, /* PB3 */
    {&gpiob,   NULL,   4, 0}, /* PB4 */
    {&gpiob,   NULL,   5, 0}, /* PB5 */
    {&gpiob, &timer4,   6, 1}, /* PB6 */
    {&gpiob, &timer4,   7, 2}, /* PB7 */	
    {&gpiob, &timer4,   8, 3}, /* PB8 */	
    {&gpiob, &timer4,   9, 4}, /* PB9 */	
    {&gpiob,   NULL, 10, 0}, /* PB10 */	
    {&gpiob,   NULL,  11, 0}, /* PB11 */
    {&gpiob,   NULL,  12, 0}, /* PB12 */
    {&gpiob,   NULL,  13, 0}, /* PB13 */
    {&gpiob,   NULL,  14, 0}, /* PB14 */
    {&gpiob,   NULL,  15, 0}, /* PB15 */

	{&gpioc,   NULL,  13, 0}, /* PC13 */	
    {&gpioc,   NULL,  14, 0}, /* PC14 */
    {&gpioc,   NULL,  15, 0}, /* PC15 */
};

uint8_t pwm_config(uint8_t pwm_num, uint8_t pin)
{
	  uint8_t sucess=0;
	  if (pwm_num<PWM_MAX_NUM)
	  {
		pwm_pin_map[pwm_num]=pin;
		timer_dev *dev = PIN_MAP[pin].timer_device;
		uint8 cc_channel = PIN_MAP[pin].timer_channel;
		
		if(started_timer[get_timer_num(dev)]!=1)
		{
		 timer_init(dev);
		 timer_pause(dev);
		 timer_gen_reg_map *regs=dev->regs.gen;
		 regs->CR1 |= (1U<<TIMER_CR1_ARPE_BIT); /*Limpa registros anteriores*/
		 regs->PSC = 1; /*Configura o prescaler*/
		 regs->ARR = 0xFFFF; /*Configura o bit de auto-recarregamento*/
		 regs->EGR |= (1U<<TIMER_EGR_UG_BIT);  /*Habilita a saida*/
		 if (dev == TIMER1)
		  {
			dev->regs.adv->BDTR |= (1U<<TIMER_BDTR_MOE_BIT); /*Habilita saida no controle-mestre do timer1*/
			dev->regs.adv->BDTR |= (1U<<TIMER_BDTR_AOE_BIT);; /*Desliga o deadtime*/
		  }
		 started_timer[get_timer_num(dev)]=1;
		}
		  
		ASSERT(dev && cc_channel);
		timer_set_mode(dev, cc_channel, TIMER_PWM);
		
		
		struct bit_and_port porta_gpio = getPort(pin);
		if(port_init[porta_gpio.indice_porta]==0) 
		{ /* Inicializa porta apenas se ela ainda não foi inicializada */
			gpio_init(porta_gpio.dev_ptr);
			port_init[porta_gpio.indice_porta]=1;
		}
		gpio_set_mode(porta_gpio.dev_ptr,porta_gpio.bit, GPIO_AF_OUTPUT_PP);
	  }
	  else
	  {
		/*"Pin out of bounds!"*/
	  }
	return sucess;
}
uint8_t pwm_on(uint8_t pwm_num) // ligar
{
  uint8_t sucess=0;
  if (pwm_num<PWM_MAX_NUM)
  {
	uint8_t pin = pwm_pin_map[pwm_num];
	timer_dev *timer = PIN_MAP[pin].timer_device;
	timer_gen_reg_map *regs=timer->regs.gen;
	regs->CR1 |= (1U<<TIMER_CR1_CEN_BIT);
  }
  else
  {
	/*"Pin out of bounds!"*/
  }
	return sucess;
}
uint8_t  pwm_off(uint8_t pwm_num) // desligar
{
  uint8_t sucess=0;
  if (pwm_num<PWM_MAX_NUM)
  {
	uint8_t pin = pwm_pin_map[pwm_num];
	timer_dev *timer = PIN_MAP[pin].timer_device;
	timer_gen_reg_map *regs=timer->regs.gen;
	regs->CR1 ^= (1U<<TIMER_CR1_CEN_BIT);
  }
  else
  {
	/*"Pin out of bounds!"*/
  }
  return sucess;
}
uint8_t  pwm_set(uint8_t pwm_num, uint16_t duty) //trocar duty cycle (razão cíclica)
{
	  uint8_t sucess=0;
	  if (pwm_num<PWM_MAX_NUM)
	  {
		uint8_t pin = pwm_pin_map[pwm_num];
		timer_dev *dev = PIN_MAP[pin].timer_device;
		uint8 cc_channel = PIN_MAP[pin].timer_channel;
		ASSERT(dev && cc_channel);
		timer_set_compare(dev, cc_channel, duty);
	  }
	  else
	  {
		/*"Pin out of bounds!"*/
	  }
	return sucess;
}

uint8_t get_timer_num(timer_dev *dev)
{
  uint8_t timer_num=0;
	
	if(dev == TIMER1)
	{
	  timer_num=1;
	}
	else if (dev == TIMER2)
	{
	  timer_num=2;
	}
	else if (dev == TIMER3)
	{
	  timer_num=3;
	}
	else if (dev == TIMER4)
	{
	  timer_num=4;
	}
	  return timer_num;
}