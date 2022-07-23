#include <stdint.h>
#include <stdbool.h>

#include "adc-driver.h"

static uint8_t started_ADC[]={0,0,0}; /* Há 3 termos para o ídice coincidir com o numero do ADC*/
extern const uint8 adc_pin_map[] = { PA0,PA1,PA2,PA3,PA4,PA5,PA6,PA7,PB0,PB1 };

adc_smp_rate get_sample_time(float);

adc_dev* getADC(uint8_t adc_num);

uint8_t adc_config(uint8_t adc_num, uint8_t adc_chan, uint8_t adc_res, uint16_t adc_freq)
{
  	uint8_t sucesso=0;
	
	/*o adc_res não é usado pois o STM32f1 utiliza o ADC sempre com 12 bits*/  
		
	if(adc_num>=1&&adc_num<=2)
	{
		adc_dev *dev=getADC(adc_num);
		if(started_ADC[adc_num]!=1)
		{
		  	adc_set_prescaler(ADC_PRE_PCLK2_DIV_6);
			adc_init(dev);
			adc_set_extsel(dev, ADC_SWSTART);
    		adc_set_exttrig(dev, 1);
			adc_enable(dev); 
			adc_calibrate(dev);
			started_ADC[adc_num]=1;
		}
		if (adc_chan<16) //adicionar suporte a leitura de GPIOs
		{
		  	uint8_t num_gpio = MAX_NUM-MAX_LEDS-(adc_chan+16*(adc_num-1));
			gpio_config(num_gpio, adc_pin_map[num_gpio], GPIO_INPUT_ANALOG);
		}
		
		adc_set_sample_rate(dev, get_sample_time(1.0/adc_freq));
		
		sucesso=1;
	}
	return sucesso;
}

uint16_t adc_read(uint8_t adc_num, uint8_t adc_chan)
{
	uint16_t saida=0;
	if(adc_num>=1&&adc_num<=2)
	{
		adc_dev *dev=getADC(adc_num);
		saida = adc_reads(dev, adc_chan);
	}
	return saida;
}

adc_dev* getADC(uint8_t adc_num)
{
	adc_dev *ADC = NULL;
	switch (adc_num)
	{
		case 1: 
		ADC = ADC1;
		break;
		case 2:
		ADC = ADC2;
		break;
	}
	  return ADC;
}

adc_smp_rate get_sample_time(float sample_time)
{
	adc_smp_rate sample_time_escolhido;
	
	if(239.5*(float)6/F_CPU<sample_time)
	{
	  sample_time_escolhido=ADC_SMPR_239_5;
	}
	else if(71.5*(float)6/F_CPU<sample_time)
	{
	  sample_time_escolhido=ADC_SMPR_71_5;
	}
	else if(55.5*(float)6/F_CPU<sample_time)
	{
	  sample_time_escolhido=ADC_SMPR_55_5;
	}
	else if(41.5*(float)6/F_CPU<sample_time)
	{
	  sample_time_escolhido=ADC_SMPR_41_5;
	} 
	else if(28.5*(float)6/F_CPU<sample_time)
	{
	  sample_time_escolhido=ADC_SMPR_28_5;
	}
	else if(13.5*(float)6/F_CPU<sample_time)
	{
	  sample_time_escolhido=ADC_SMPR_13_5;
	}
	else if(7.5*(float)6/F_CPU<sample_time)
	{
	  sample_time_escolhido=ADC_SMPR_7_5;
	}
	else if(1.5*(float)6/F_CPU<sample_time)
	{
	  sample_time_escolhido=ADC_SMPR_1_5;
	}

	return sample_time_escolhido;
}
