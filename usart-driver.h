#include "Core\usart.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <yfuns.h>

uint8_t uart_config(uint8_t uart_num, uint32_t uart_baudrate);

size_t __read(int handle, unsigned char * buffer, size_t size);

size_t __write(int handle, const unsigned char * buffer, size_t size);

bool selectUART(uint8_t uart_num);

bool buffer_empty(uint8_t num_uart, char buf_type);

void clear_buffer(uint8_t num_uart, char buf_type);
