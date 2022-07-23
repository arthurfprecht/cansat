#include <stdint.h>
#include "gpio-driver.h"

#define MAX_LEDS 16 /* Leds de 0 a MAX_LEDS-1 */

uint8_t led_config(uint8_t led_num, uint8_t pin);

uint8_t led_on(uint8_t led_num); //Ligar

uint8_t led_off(uint8_t led_num); //Desligar

uint8_t led_toggle(uint8_t led_num); //Alternar

