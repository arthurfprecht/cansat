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

#define TAM_FILTRO 5

bmp280 bmp_280; /* Instância do BMP280 com endereço padrão */
uint8_t MPU_addr = 0x68; /* Endereço do MPU6050 utilizado o*/
RF24 radio(PB0, PA4); /* Instância do NRF24L01 nos pinos (CE, CS) do Arduino */

const uint64_t pipe = 68; //Endereço para receber os dados

const uint8_t led_r = PB4, led_g = PA15, led_b = PB3, buz = PB9;

static xQueueHandle fila_dados_lidos;
static xQueueHandle fila_dados_processados;
static xTimerHandle timer;

static xTaskHandle heartbeat_handle, tarefa_leitura_handle, 
				   tarefa_processamento_handle, tarefa_envio_handle;

typedef struct 
{
  uint32_t tempo;
  int16_t ac[3], gyr[3], mag[3];
  float temp, pres;
}packet;

typedef struct //Define a estrutura de dados do Ring Buffer que guardará as "tamFiltro" ultimas leituras do ultrassom
{
  uint8_t indice = 0; //Salva em qual posicao do vetor foi colocada a ultima leitura
  packet dados_recebidos[TAM_FILTRO]; //Vetor que guarda as leituras
} rb_leituras;

void ringBuf (rb_leituras *dados_lidos, packet leitura);
packet mediaMovel(rb_leituras *dados_lidos, const double pesos[]);

static void ISR (xTimerHandle xTimer)
{
 	vTaskResume(tarefa_leitura_handle);
}

static void heartbeat(void *pvParameters) 
{
	while(1)
	{
        vTaskDelay(1000);
        led_on(1);
        vTaskDelay(1000);
        led_off(1);
    }
}

static void tarefa_leitura(void *pvParameters)
{
	packet raw;
	
	static uint32_t tempo_inicial = millis();
	
	while(1)
	{
		raw.tempo = millis() - tempo_inicial;

		recebeDados(MPU_addr, raw.ac, raw.gyr);

		raw.mag[0] = 0; raw.mag[1] = 0; raw.mag[2] = 0;
		
		double temperatura_double = bmp_280.readTemperature();
		raw.pres = (float)bmp_280.readPressure(temperatura_double);
		raw.temp = (float)temperatura_double;
	
		xQueueSendToBack(fila_dados_lidos, (void*)&raw, 0);
		vTaskSuspend(tarefa_leitura_handle);
		vTaskResume(tarefa_processamento_handle);

	    taskYIELD();
    }
}

static void tarefa_processamento(void *pvParameters)
{
  	static rb_leituras processamento;
	packet dados_recebidos;
	
  	while(1)
	{
	  	xQueueReceive(fila_dados_lidos,&dados_recebidos,0);
		ringBuf(&processamento, dados_recebidos);
				
		const double pesos[] = {0.6, 0.2, 0.1, 0.05, 0.05};
		packet a_enviar = mediaMovel(&processamento, pesos);
		
		printf("Tempo: %.3fs\n", (double)(dados_recebidos.tempo)/1000);
		while(!buffer_empty(1, 'w'));
		
	    printf("Temp: %.1foC \tPres: %.3f kPa \t\n",
			 dados_recebidos.temp, dados_recebidos.pres/1000);
	    while(!buffer_empty(1, 'w'));
		double acc[3], gyro[3];
		
		converteDados(dados_recebidos.ac, dados_recebidos.gyr, acc, gyro);
	    exibeMPU(acc, gyro);

		xQueueSendToBack(fila_dados_processados, (void*)&dados_recebidos, 0);
		vTaskSuspend(tarefa_processamento_handle);
		vTaskResume(tarefa_envio_handle);
		taskYIELD();
    }
}

static void tarefa_envio(void *pvParameters)
{
  	packet dados_processados;
  	while(1)
	{	
	  xQueueReceive(fila_dados_processados,&dados_processados,0);
	  
	  radio.write(&dados_processados, sizeof(dados_processados));
	  
	  vTaskSuspend(tarefa_envio_handle);
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
	
	radio.openWritingPipe(pipe);  //Entra em modo de transmissao
	radio.setDataRate(RF24_250KBPS);
  	radio.stopListening(); //NÃO ESQUECER This sets the module as transmitter!!!!!
	
	fila_dados_lidos = xQueueCreate(10, sizeof(packet));
	fila_dados_processados = xQueueCreate(10, sizeof(packet));
	
	xTaskCreate(heartbeat, "Task1", configMINIMAL_STACK_SIZE,
                NULL, tskIDLE_PRIORITY + 1, &heartbeat_handle);
	
	xTaskCreate(tarefa_leitura, "Task2", 500,
                NULL, tskIDLE_PRIORITY + 2, &tarefa_leitura_handle);
	vTaskSuspend(tarefa_leitura_handle);	  
	xTaskCreate(tarefa_processamento, "Task3", 1000,
                NULL, tskIDLE_PRIORITY + 2, &tarefa_processamento_handle);
	vTaskSuspend(tarefa_processamento_handle);
	xTaskCreate(tarefa_envio, "Task4", 500,
                NULL, tskIDLE_PRIORITY + 3, &tarefa_envio_handle);
	vTaskSuspend(tarefa_envio_handle);
	
	timer = xTimerCreate("Timer_20Hz", 50, true, 0, ISR); 
	
	xTimerStart(timer,100);
	
    vTaskStartScheduler();
	
  	while(1)
	{
	  
	}
	
  	return 0;
}

/* Tarefas auxiliares para o processamento de dados */

void ringBuf (rb_leituras *dados_lidos, packet leitura) //Função que guarda a leitura no Ring Buffer
{
  if (dados_lidos->indice > TAM_FILTRO - 2) //Se o último índice+1 fosse chegar a "tamFiltro"
    dados_lidos->indice = 0; //Faz o índice voltar ao início, 0
  else
    dados_lidos->indice++; //Incrementa o último indice para colocar a leitura na próxima posição
  dados_lidos->dados_recebidos[dados_lidos->indice] = leitura; //Coloca a leitura no índice correto
}

packet mediaMovel(rb_leituras *dados_lidos, const double pesos[])
{
  packet media_movel;
  
  media_movel.tempo = (dados_lidos->dados_recebidos[dados_lidos->indice]).tempo;
  
  double soma_pesos=0;
  float temp_ac = 0, pres_ac = 0;
  int32_t acc_ac[3] = {0,0,0}, gyro_ac[3] = {0,0,0}, mag_ac[3] = {0,0,0};
  
  for(uint8_t amostra = 0; amostra<TAM_FILTRO; amostra++)
  {
	soma_pesos+=pesos[amostra];
	
	uint8_t indice_rb = (TAM_FILTRO + dados_lidos->indice - amostra) % TAM_FILTRO;
	
	temp_ac += (dados_lidos->dados_recebidos[indice_rb]).temp;
	pres_ac += (dados_lidos->dados_recebidos[indice_rb]).pres;
	
	for (uint8_t eixo = 0; eixo < 3; eixo++)
	{
	  acc_ac[eixo] += (dados_lidos->dados_recebidos[indice_rb]).ac[eixo];
	  gyro_ac[eixo] += (dados_lidos->dados_recebidos[indice_rb]).gyr[eixo];
	  mag_ac[eixo] += (dados_lidos->dados_recebidos[indice_rb]).mag[eixo];
	}
  }
  
  for (uint8_t eixo = 0; eixo < 3; eixo++)
  {
	media_movel.ac[eixo] = round(acc_ac[eixo] / (soma_pesos * TAM_FILTRO));
	media_movel.gyr[eixo] = round(gyro_ac[eixo] / (soma_pesos * TAM_FILTRO));
	media_movel.mag[eixo] = round(mag_ac[eixo] / (soma_pesos * TAM_FILTRO));
  }
  
  media_movel.temp = temp_ac / (soma_pesos * TAM_FILTRO);
  media_movel.pres = pres_ac / (soma_pesos * TAM_FILTRO);
  
  return media_movel;
}
