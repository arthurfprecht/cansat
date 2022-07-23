#include <stdint.h>
#include <stdbool.h>

#include "timer-driver.h"

volatile uint32_t time_miliseconds=0;

bool started_timer[]={0,0,0,0,0};

void increaseTime(void);

inline uint8_t get_nvic_num(uint8_t timer_num){return NVIC_TIMER2+(timer_num-2);}

timer_dev* get_timer_struct(uint8_t timer_num);

uint8_t timer_config(uint8_t timer_num, uint16_t period_ms)
{
	uint8_t sucesso=0;
	  
	if((timer_num>=1)&&(timer_num<=4)&&(period_ms!=0))
	{

	  	if(started_timer[timer_num]==0)
		{
		  timer_dev *timer = get_timer_struct(timer_num);
		  timer_init(timer);
		  timer_gen_reg_map *regs=timer->regs.gen;
		  regs->CR1 |= (1U<<TIMER_CR1_ARPE_BIT);
		  regs->PSC = (unsigned)(36000-1); 
		  regs->ARR = (unsigned)(2*period_ms-1);
		  if (timer == TIMER1)
		  {
			timer->regs.adv->BDTR |= (1U<<TIMER_BDTR_MOE_BIT); /* Enable da saida do timer1 */
			timer->regs.adv->BDTR |= (1U<<TIMER_BDTR_AOE_BIT);; /* Set the Dead Time value to 0 */
		  }
		  regs->CR1 |= (1U<<TIMER_CR1_URS_BIT); /*gera int no overflow*/
		  regs->EGR |= (1U<<TIMER_EGR_UG_BIT);  
		  timer_attach_interrupt(timer, timer_num, increaseTime); 
		  started_timer[timer_num]=1;
		}
	
	}
	else
	{
		/* Timer não configurado */
	}
	return sucesso;
}

uint8_t timer_start(uint8_t timer_num)
{
  	uint8_t sucesso=0;

	if((timer_num>=1)&&(timer_num<=4))
	{
		timer_dev *timer = get_timer_struct(timer_num);
		timer_gen_reg_map *regs=timer->regs.gen;
		regs->CR1 |= (1U<<TIMER_CR1_CEN_BIT);
	}
	else
	{
		/* Timer não ativado */
	}
	return sucesso;
}

uint8_t timer_stop(uint8_t timer_num)
{
  	uint8_t sucesso=0;

	if((timer_num>=1)&&(timer_num<=4))
	{
		timer_dev *timer = get_timer_struct(timer_num);
		timer_gen_reg_map *regs=timer->regs.gen;
		regs->CR1 ^= (1U<<TIMER_CR1_CEN_BIT);
	}
	else
	{
		/* Timer não desativado */
	}
	return sucesso;
}

uint32_t timer_count(uint8_t timer_num)
{
	uint32_t count = 0;
	
	if((timer_num>=1)&&(timer_num<=4))
	{
		timer_dev *timer = get_timer_struct(timer_num);
		
		count = timer->regs.gen->CNT;
	}
	return count;
}

void increaseTime(void)
{
  time_miliseconds++;
}

uint8_t start_millis(uint8_t timer_num)
{
  uint8_t sucesso = 0;
	if (timer_config(timer_num, 1))
	{
	  if(timer_start(timer_num))
	  {
		sucesso=1;
	  }
	}	
  return sucesso;
}

uint32_t millis()
{
  return time_miliseconds; 
}

timer_dev* get_timer_struct(uint8_t timer_num)
{
	timer_dev *timer = NULL;
	switch(timer_num)
		{
			case 1:
		 		timer = TIMER1;
				break;
			case 2:
				timer = TIMER2;
				break;
			case 3:
				timer = TIMER3;
				break;	
			case 4:
				timer = TIMER4;
				break;
		}
	return timer;
}
