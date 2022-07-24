#include <stdint.h>
#include <stdbool.h>

#include "Core\sysInit.h"

#include "gpio-driver.h"
#include "led-driver.h"
#include "timer-driver.h"
#include "pwm-driver.h"
#include "adc-driver.h"
#include "usart-driver.h"

#include "SPI/SPI.h"
#include "Wire/Wire.h"
#include "Wire/I2C.h"

uint8_t MPU_addr = 0x68, i2cData[14];

/*Dados brutos recebidos do sensor */
int16_t accX, gyroX; // Aceleração e vel angular do eixo X (não utilizado, rotação no plano)
int16_t accY, gyroY; // Aceleração e vel angular do eixo Y (reservado para uso futuro, inclinação lateral do corpo)
int16_t accZ, gyroZ; // Aceleração e vel angular do eixo Z (Utilizado, angulo da coxa em relação ao solo)

/*Angulos calculados a partir de cada sensor)*/
double accZangle, accYangle; // angulo calculado a partir dos dados do acelerometro
double gyroZangle, gyroYangle; // angulo calculado a partir dos dados do giroscópio

void inicializaMPU(int MPU) //MPU é o endereço I2C sendo utilizado pelo MPU em questão, em Hexadecimal
{
  selectAdress(MPU); // Seleciona o endereço do sensor que está recebendo a comunicação
  printf("Start:\n");
  i2cData[0] = 7; // Seta a rate de amostatragem para 1000Hz - 8kHz/(7+1) = 1000Hz
  i2cData[1] = 0x00;
  i2cData[2] = 0x00; // Seta a range maxima do Giroscopio para ±250 graus/s
  i2cData[3] = 0x00; // Seta a range maxima do Acelerometro para ±2g
  while (i2cWrite(0x19, i2cData, 4, false));
  while (i2cWrite(0x6B, 0x01, true));
  while (i2cRead(0x75, i2cData, 1));
  printf("Endereço: %x \n", i2cData[0]); // Mostra o endereço do sensor na tela
  if (i2cData[0] != 0x68) //Verifica se a resposta de qual o endereço o sensor tem está correta.
  {
    printf("Error reading sensor\n");
    while (1);
  }
  delay(100); // Aguarda o sensor se estabilizar
}

/*Função responsavel por realizar a leitura do MPU*/
bool recebeDados(int MPU) //MPU é o endereço I2C sendo utilizado pelo MPU em questão, em Hexadecimal
{
  selectAdress(MPU);  // Seleciona o endereço do sensor que está recebendo a comunicação
  /*Recebimento dos valores lidos pelo sensor por  meio do buffer I2C*/
  if (i2cRead(0x3B, i2cData, 14) == 0) //Se a leitura dos dados dos sensores ocorre com sucesso, os valores de cada grandeza são armazenados
  {
    accX = ((i2cData[0] << 8) | i2cData[1]); // Recebimento do valor bruto da aceleração do eixo X (não utilizado, pois o X é no mesmo plano que o chão)
    accY = ((i2cData[2] << 8) | i2cData[3]); // Recebimento do valor bruto da aceleração do eixo Y (reservado para uso futuro, inclinação lateral do corpo)
    accZ = ((i2cData[4] << 8) | i2cData[5]); // Recebimento do valor bruto da aceleração do eixo Z (Utilizado, angulo da coxa em relação ao solo)
    gyroX = ((i2cData[8] << 8) | i2cData[9]); // Recebimento do valor bruto da velocidade angular do eixo X (não utilizado, pois o X é no mesmo plano que o chão)
    gyroY = ((i2cData[10] << 8) | i2cData[11]); // Recebimento do valor bruto da velocidade angular do eixo Y (reservado para uso futuro, inclinação lateral do corpo)
    gyroZ = ((i2cData[12] << 8) | i2cData[13]); // Recebimento do valor bruto da velocidade angular do eixo Z (Utilizado, angulo da coxa em relação ao solo)

	return true; 
  }
  else // caso a função não tenha sucesso, é retornado um "falso"
  {
    return 0;
  }
}

int main()
{
  	systemInit();
  	uart_config(1, 57600);
	
	printf("Iniciando teste de I2C: \n");
	while(!buffer_empty(1, 'w'));
	
	Wire.begin();
	start_millis(2); 
	
	inicializaMPU(MPU_addr);
	
  	while(1)
	{
	  recebeDados(MPU_addr);
	  double Ax1 = accX * 0.000598754, Ay1 = accY * 0.000598754,  Az1 = accZ * 0.000598754; //A cte vem de 9.81/16384
      double Gx1 = gyroX * 0.007633587, Gy1 = gyroY * 0.007633587,  Gz1 = gyroZ * 0.007633587; //A cte vem de 1/131

	  printf("acc [x,y,z] = [%f, %f, %f] m/s^2 \n", Ax1, Ay1, Az1);
	  while(!buffer_empty(1, 'w'));
	  printf("gyro [x,y,z] = [%f, %f, %f] rad/s \n", Gx1, Gy1, Gz1);
	  while(!buffer_empty(1, 'w'));
	}
  	return 0;
}
