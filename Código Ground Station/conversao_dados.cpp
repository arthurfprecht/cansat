#include "conversao_dados.h"

const double convAccel = 0.000598754; //A cte vem de 9.81/16384
const double convGyro = 0.007633587; //A cte vem de 1/131
const double convMag = 0.0009090909; //A cte vem de 1/1100

uint32_t startMicros = 0;

void processData(packet raw, data_t* dadosConvertidos)
{
  dadosConvertidos->tim = (unsigned long)raw.tempo;
  for (byte i = 0; i < 3; i++)
  {
    dadosConvertidos->acc[i] = raw.ac[i] * convAccel; //Converte o valor bruto de aceleração para o valor em m/s
    dadosConvertidos->gyro[i] = raw.gyr[i] * convGyro; //Converte o valor bruto de vel. ang. para o valor em º/s
    dadosConvertidos->magn[i] = raw.mag[i] * convMag; //Converte o valor bruto de campo magnético para gauss
  }
  dadosConvertidos->temp = raw.temp;
  dadosConvertidos->pres = raw.pres;
  dadosConvertidos->alt = bme.calcAltitude(dadosConvertidos->pres, 1021.0);
}

void printHeader(Print * pr) //Print data header.
{
  startMicros = 0;
  pr->println(F("tempo,ax,ay,az,gx,gy,gz,mx,my,mz,temp,pres,alt"));
}

void printData(Print * pr, data_t* dataSD) //Print a data record.
{
  pr->write('\n');
  pr->printf("%.3f", (float)(dataSD->tim)/1000.0);  pr->write('\t');
  for (byte i = 0; i < 3; i++) {
    pr->print(dataSD->acc[i]);  pr->write('\t');
  } for (byte i = 0; i < 3; i++) {
    pr->print(dataSD->gyro[i]);  pr->write('\t');
  } for (byte i = 0; i < 3; i++) {
    pr->print(dataSD->magn[i]);  pr->write('\t');
  }
  pr->printf("%.3f",dataSD->temp );  pr->write('\t');
  pr->printf("%.3f",dataSD->pres );  pr->write('\t');
  pr->print(dataSD->alt);  pr->write('\t');
}
