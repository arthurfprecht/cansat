#include <stdint.h>
#include <stdbool.h>

#include "Core\sysInit.h"

#include "gpio-driver.h"
#include "led-driver.h"
#include "timer-driver.h"
#include "pwm-driver.h"
#include "adc-driver.h"
#include "usart-driver.h"

#include "FreeRTOS/MapleFreeRTOS.h"
#include "FreeRTOS/utility/timers_rtos.h"

#include "SPI/SPI.h"
#include "Wire/Wire.h"
#include "MPU6050/MPU6050.h"
#include "BMP280/BMP280-library.h"
#include "RF24/RF24.h"

bmp280 bmp_280; /* Instância do BMP280 com endereço padrão */
uint8_t MPU_addr = 0x68; /* Endereço do MPU6050 utilizado o*/
RF24 radio(PB0, PA4); /* Instância do NRF24L01 nos pinos (CE, CS) do Arduino */

const uint64_t pipe_64 = 68;   //Endereço para receber os dados

const uint8_t led_r = PB4, led_g = PA15, led_b = PB3, buz = PB9;

typedef struct 
{
  uint32_t tempo;
  int16_t ac[3], gyr[3], mag[3];
  int32_t temp, pres;
}packet;

static xQueueHandle fila_dados_sensor;
static xTimerHandle timer;

volatile bool timer_ativado = 0;

static void ISR (xTimerHandle xTimer)
{
 	timer_ativado = 1; 
}

static void vLEDFlashTask1(void *pvParameters) {
	while(1)
	{
        vTaskDelay(1000);
        led_on(1);
        vTaskDelay(1000);
        led_off(1);
    }
}

static void sensorReadTask(void *pvParameters)
{
	
	static long tempo_inicial = millis();
	while(1)
	{
	  if(timer_ativado == 1)
	  {
		packet raw;		raw.pres=0;
		
		raw.tempo = millis() - tempo_inicial;

		recebeDados(MPU_addr, raw.ac, raw.gyr);

		raw.mag[0] = 0; raw.mag[1] = 0; raw.mag[2] = 0;
		
		long temperatura;
		raw.pres = bmp_280.readRawPressure(temperatura);
		raw.temp = temperatura;
		
		xQueueSendToBack(fila_dados_sensor, (void*)&raw, 0);
		
		radio.write(&raw, sizeof(raw));
	  }
	  taskYIELD();
    }
}

static void sensorShowTask(void *pvParameters)
{
  	packet dados_recebidos;
  	while(1)
	{
	  if(xQueueReceive(fila_dados_sensor, (void*)&dados_recebidos, 0) != errQUEUE_EMPTY)
	  {
		double temperatura_recebida = bmp_280.calcTemperature(dados_recebidos.temp);
		double pressao_recebida = bmp_280.calcPressure(dados_recebidos.pres, temperatura_recebida*5120.0);
		
		printf("Tempo: %.3fs\n", (double)(dados_recebidos.tempo)/1000);
		while(!buffer_empty(1, 'w'));
	    printf("Temperatura: %.3foC \tPressao: %.3f kPa \t\n",
			 temperatura_recebida, pressao_recebida/1000);
	    while(!buffer_empty(1, 'w'));
		double acc[3], gyro[3];
		converteDados(dados_recebidos.ac, dados_recebidos.gyr, acc, gyro);
	    exibeMPU(acc, gyro);
	  }
	  
	  
	  taskYIELD();
    }
}

int main()
{
  	systemInit();
	start_millis(2); 
  	uart_config(1, 57600);
	led_config(0, led_r);
	led_config(1, led_g);
	led_config(2, led_b);
	led_config(3, buz);
	
	led_off(0); led_off(2); led_off(3);
	
	printf("Iniciando teste de transmissao: \n");
	while(!buffer_empty(1, 'w'));
	
	bmp_280.begin();
	inicializaMPU(MPU_addr); 
	radio.begin();

	
	radio.openWritingPipe(pipe_64);  //Entra em modo de transmissao
  	radio.stopListening(); //NÃO ESQUECER This sets the module as transmitter!!!!!
	
	fila_dados_sensor = xQueueCreate(10, sizeof(packet));
	
	xTaskCreate(vLEDFlashTask1, "Task1", configMINIMAL_STACK_SIZE,
                NULL, tskIDLE_PRIORITY + 3, NULL);
	
	xTaskCreate(sensorReadTask, "Task2", 1000,
                NULL, tskIDLE_PRIORITY + 3, NULL);
	
	xTaskCreate(sensorShowTask, "Task3", 1000,
                NULL, tskIDLE_PRIORITY + 3, NULL);
	
	timer = xTimerCreate("Timer_20Hz", 50, true, 0, ISR); 
    
	xTimerStart(timer,10);
	
    vTaskStartScheduler();
	
  	while(1)
	{
	}
	
  	return 0;
}
