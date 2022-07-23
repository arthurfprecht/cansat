#include <stdint.h>
#include <stdbool.h>

#include "Core\timer.h"
#include "timer-driver.h"

typedef struct stm32_pin_info {
    gpio_dev *gpio_device;      /**< Maple pin's GPIO device */
    timer_dev *timer_device;    /**< Pin's timer device, if any. */
    uint8 gpio_bit;             /**< Pin's GPIO port bit. */
    uint8 timer_channel;        /**< Timer channel, or 0 if none. */
} stm32_pin_info;

extern const stm32_pin_info PIN_MAP[];

uint8_t pwm_config(uint8_t pwm_num, uint8_t pin);
uint8_t pwm_on(uint8_t pwm_num); // ligar
uint8_t pwm_off(uint8_t pwm_num); // desligar
uint8_t pwm_set(uint8_t pwm_num, uint16_t duty); //trocar duty cycle (razão cíclica)
