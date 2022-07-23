#include <stdint.h>
#include <stdbool.h>

#include "Core\adc.h"
#include "Core\util_adc.h"
#include "gpio-driver.h"
#include "led-driver.h"

extern adc_dev* getADC(uint8_t adc_num);

uint8_t adc_config(uint8_t adc_num, uint8_t adc_chan, uint8_t adc_res, uint16_t adc_freq);

uint16_t adc_read(uint8_t adc_num, uint8_t adc_chan);
	
