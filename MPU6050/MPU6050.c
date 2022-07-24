#include <stdint.h>
#include <stdbool.h>
#include "MPU6050.h"

uint8_t i2cData[14];

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
bool recebeDados(int MPU, int16_t acc[], int16_t gyro[]) //MPU é o endereço I2C sendo utilizado pelo MPU em questão, em Hexadecimal
{
  selectAdress(MPU);  // Seleciona o endereço do sensor que está recebendo a comunicação
  /*Recebimento dos valores lidos pelo sensor por  meio do buffer I2C*/
  if (i2cRead(0x3B, i2cData, 14) == 0) //Se a leitura dos dados dos sensores ocorre com sucesso, os valores de cada grandeza são armazenados
  {
    acc[0] = ((i2cData[0] << 8) | i2cData[1]); // Recebimento do valor bruto da aceleração do eixo X (não utilizado, pois o X é no mesmo plano que o chão)
    acc[1] = ((i2cData[2] << 8) | i2cData[3]); // Recebimento do valor bruto da aceleração do eixo Y (reservado para uso futuro, inclinação lateral do corpo)
    acc[2] = ((i2cData[4] << 8) | i2cData[5]); // Recebimento do valor bruto da aceleração do eixo Z (Utilizado, angulo da coxa em relação ao solo)
    gyro[0] = ((i2cData[8] << 8) | i2cData[9]); // Recebimento do valor bruto da velocidade angular do eixo X (não utilizado, pois o X é no mesmo plano que o chão)
    gyro[1]= ((i2cData[10] << 8) | i2cData[11]); // Recebimento do valor bruto da velocidade angular do eixo Y (reservado para uso futuro, inclinação lateral do corpo)
    gyro[2] = ((i2cData[12] << 8) | i2cData[13]); // Recebimento do valor bruto da velocidade angular do eixo Z (Utilizado, angulo da coxa em relação ao solo)

	return true; 
  }
  else // caso a função não tenha sucesso, é retornado um "falso"
  {
    return 0;
  }
}

void converteDados(const int16_t acc[], const int16_t gyro[], 
				   double acc_conv[], double gyro_conv[] )
{
  /*
if ((sizeof(acc)/sizeof(int16_t)==3) && (sizeof(acc_conv)/sizeof(double)==3) &&
	(sizeof(gyro)/sizeof(int16_t)==3) && (sizeof(gyro_conv)/sizeof(double)==3))
	{*/
	  for (uint8_t eixo = 0; eixo<3; eixo++)
	  {
		  acc_conv[eixo] = acc[eixo] * 0.000598754; /* Cte vem de 9.81/16384 */
		  gyro_conv[eixo] = gyro[eixo] * 0.007633587; /* Cte vem de 1/131 */
	  }
	/*}*/
}

/*uint8_t MPU_addr = 0x68;*/
void testaMPU(uint8_t MPU_addr)
{
	  int16_t acc[3], gyro[3]; /* Aceleração e vel angular brutas */
	  double acc_c[3], gyro_c[3];
		
	  recebeDados(MPU_addr, acc, gyro);
	  converteDados(acc, gyro, acc_c, gyro_c);

	  printf("acc [x,y,z] = [%f, %f, %f] m/s^2 \n", acc_c[0], acc_c[1], acc_c[2]);
	  while(!buffer_empty(1, 'w'));
	  printf("gyro [x,y,z] = [%f, %f, %f] rad/s \n", gyro_c[0], gyro_c[1], gyro_c[2]);
	  while(!buffer_empty(1, 'w'));
}