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

bmp280 bmp_280;
uint8_t MPU_addr=0x68;

typedef struct 
{
  uint32_t tempo;
  double Temp, Pres, Alt;
  double acc[3], gyro[3];
} dados_sensor;

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
        led_on(0);
        vTaskDelay(1000);
        led_off(0);
    }
}

static void vLEDFlashTask2(void *pvParameters) {
	while(1)
	{
        vTaskDelay(2000);
        led_on(1);
        vTaskDelay(2000);
        led_off(1);
    }
}

static void sensorReadTask(void *pvParameters)
{
	dados_sensor dados_lidos;
	static long tempo_inicial = millis();
	while(1)
	{
	  if(timer_ativado == 1)
	  {
		dados_lidos.tempo = millis()-tempo_inicial;
		dados_lidos.Temp = bmp_280.readTemperature();
		dados_lidos.Pres = bmp_280.readPressure()*1000;
		dados_lidos.Alt = bmp_280.calcAltitude(dados_lidos.Pres,1021000); // this should be adjusted to your local forcase
		
		int16_t acc[3], gyro[3]; /* Aceleração e vel angular brutas */
		recebeDados(MPU_addr, acc, gyro);
		converteDados(acc, gyro, dados_lidos.acc, dados_lidos.gyro);
		timer_ativado = 0;
	   
		xQueueSendToBack(fila_dados_sensor, (void*)&dados_lidos, 0);
	  }
	  taskYIELD();
    }
}

static void sensorShowTask(void *pvParameters)
{
  	dados_sensor dados_recebidos;
  	while(1)
	{
	  if(xQueueReceive(fila_dados_sensor, (void*)&dados_recebidos, 0) != errQUEUE_EMPTY)
	  {
		printf("Tempo: %.3fs\n", (double)(dados_recebidos.tempo)/1000);
		while(!buffer_empty(1, 'w'));
	    printf("Temperatura: %.1foC \tPressao: %.3f kPa \tAltitude: %.2fm \n",
			 dados_recebidos.Temp, dados_recebidos.Pres/1000, dados_recebidos.Alt);
	    while(!buffer_empty(1, 'w'));
	    exibeMPU(dados_recebidos.acc, dados_recebidos.gyro);
	  }
	  taskYIELD();
    }
}

int main()
{
  	systemInit();
	start_millis(2); 
  	uart_config(1, 57600);
	led_config(0, PB11);
	led_config(1, PB10);
	
	printf("Iniciando teste de I2C: \n");
	while(!buffer_empty(1, 'w'));
	
	bmp_280.begin();
	inicializaMPU(MPU_addr); 
	
	fila_dados_sensor = xQueueCreate(10, sizeof(dados_sensor));
	
	xTaskCreate(vLEDFlashTask1, "Task1", configMINIMAL_STACK_SIZE,
                NULL, tskIDLE_PRIORITY + 3, NULL);
		
	xTaskCreate(vLEDFlashTask2, "Task2", configMINIMAL_STACK_SIZE,
                NULL, tskIDLE_PRIORITY + 3, NULL);
	
	xTaskCreate(sensorReadTask, "Task3", 1000,
                NULL, tskIDLE_PRIORITY + 3, NULL);
	
	xTaskCreate(sensorShowTask, "Task4", 1000,
                NULL, tskIDLE_PRIORITY + 3, NULL);
	
	timer = xTimerCreate("Timer_20Hz", 50, true, 0, ISR); 
    
	xTimerStart(timer,10);
	
    vTaskStartScheduler();
	
  	while(1)
	{
	}
	
  	return 0;
}
