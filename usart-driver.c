#include "usart-driver.h"
#include "gpio-driver.h"

static usart_dev *usart = NULL;

/* 0 adicionado no inicio do vetor para o indice concordar com o num da UART */
const uint8_t pino_tx[] = {0, PA9, PA2, PB10}, pino_rx[] = {0, PA10, PA3, PB11};

bool UART_iniciada[] = {0,0,0,0}; 

usart_dev * get_usart(uint8_t num_usart);

uint8_t uart_config(uint8_t uart_num, uint32_t uart_baudrate)
{
  uint8_t sucesso = 0;
  if((uart_num>=1) && (uart_num<=3))
  {
	usart = get_usart(uart_num);
	
	//disable_timer_if_necessary(txi->timer_device, txi->timer_channel);

    usart_init(usart);
	
	struct bit_and_port porta_pino_tx = getPort(pino_tx[uart_num]);
	if(port_init[porta_pino_tx.indice_porta]!=1) 
	{ /* Inicializa porta apenas se ela ainda não foi inicializada */
		gpio_init(porta_pino_tx.dev_ptr);
		port_init[porta_pino_tx.indice_porta]=1;
	}
	
	struct bit_and_port porta_pino_rx = getPort(pino_rx[uart_num]);
	if(port_init[porta_pino_rx.indice_porta]!=1) 
	{ /* Inicializa porta apenas se ela ainda não foi inicializada */
		gpio_init(porta_pino_rx.dev_ptr);
		port_init[porta_pino_rx.indice_porta]=1;
	}
	usart->regs->CR3 |= (1U << USART_CR3_NACK_BIT);
    usart_config_gpios_async(usart, porta_pino_rx.dev_ptr, porta_pino_rx.bit,
                             porta_pino_tx.dev_ptr, porta_pino_tx.bit, 0);
    usart_set_baud_rate(usart, F_CPU, uart_baudrate);
    usart_enable(usart);
	sucesso=1;
	UART_iniciada[uart_num]=1;
  }
	return sucesso;
}

size_t __write(int handle, const unsigned char * buffer, size_t size)
{
  size_t nChars = 0;

  if (buffer == 0)
  {
    /*
     * This means that we should flush internal buffers.  Since we
     * don't we just return.  (Remember, "handle" == -1 means that all
     * handles should be flushed.)
     */
    return 0;
  }

  /* This template only writes to "standard out" and "standard err",
   * for all other file handles it returns failure. */
  if (handle != _LLIO_STDOUT && handle != _LLIO_STDERR)
  {
    return _LLIO_ERROR;
  }

  for (/* Empty */; size != 0; --size)
  {
    if (usart_tx(usart, buffer++, 1)< 0)
    {
      return _LLIO_ERROR;
    }
    ++nChars;
  }
  return nChars;
}

size_t __read(int handle, unsigned char * buffer, size_t size)
{
  int nChars = 0;
  
  if (handle != _LLIO_STDIN)/* This template only reads from "standard in", for all other file handles it returns failure. */
  {
    return _LLIO_ERROR;
  }
  
  while(usart_data_available(usart)==0);

  int c = (int)usart_getc(usart);

  *buffer++ = c;
  ++nChars;
	
  return nChars;
}

bool selectUART(uint8_t uart_num)
{
  bool sucesso = 0;
  if (UART_iniciada[uart_num] == 1)
  {
	usart = get_usart(uart_num);
	sucesso = 1;
  }
  return sucesso;
}

bool buffer_empty(uint8_t num_uart, char buf_type)
{
  usart = get_usart(num_uart);
  bool saida = 0;
  if (buf_type == 'w')
	saida =  rb_is_empty(usart->wb);
  else if (buf_type == 'r')
	saida =  rb_is_empty(usart->rb);
  return saida;
}

void clear_buffer(uint8_t num_uart, char buf_type)
{
  usart = get_usart(num_uart);
  if (buf_type == 'w')
	rb_reset(usart->wb);
  else if (buf_type == 'r')
	rb_reset(usart->rb);
}

usart_dev * get_usart(uint8_t num_usart)
{
	usart_dev *usart_atual = NULL;
	
	switch(num_usart)
	{
		case 1:
			usart_atual = USART1;
		break;
		case 2:
			usart_atual = USART2;
		break;
		case 3:
			usart_atual = USART3;
		break;
	}
	
	return usart_atual; 
}
