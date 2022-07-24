#include <stdint.h>
#include <stdbool.h>

#include "Core/timer.h"
#include "Core/systick.h"

extern volatile uint32_t time_miliseconds;
extern uint8_t get_nvic_num(uint8_t timer_num);
extern bool started_timer[];
extern timer_dev* get_timer_struct(uint8_t timer_num);

uint8_t timer_config(uint8_t timer_num, uint16_t period_ms);
uint8_t timer_start(uint8_t timer_num);
uint8_t timer_stop(uint8_t timer_num);
uint32_t timer_count(uint8_t timer_num);

uint8_t start_millis(uint8_t timer_num);
uint32_t millis();
void delay(uint32_t delay_desejado);
uint32 micros(void);

uint8_t timer_delay(uint8_t timer_num, uint16_t period_ms);


