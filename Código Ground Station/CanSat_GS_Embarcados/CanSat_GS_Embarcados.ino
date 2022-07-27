#include "C:/Users/arthu/Documents/Arduino/CanSat/CanSatV3/UserTypes.h"

#include <SPI.h>
#include <Wire.h>
#include <farmerkeith_BMP280.h>
#include <RF24.h>

#include "C:/Users/arthu/Documents/Arduino/CanSat/CanSatV3/conversao_dados.h"

//RF24 radio(9, 10);
//RF24 radio(PB0, PA4); //Inicializa o NRF24L01 nos pinos (CE, CS) do Arduino
RF24 radio(D4, D8);
bmp280 bme; // I2C

#include "C:/Users/arthu/Documents/Arduino/CanSat/CanSatV3/conversao_dados.cpp"
/* Não é o ideal mas só assim para o compilador do ESP funcionar */

const uint64_t pipe = 68; //Endereço para receber os dados
const int LED = LED_BUILTIN;

packet recebidos; //Armazena os dados recebidos

void debugRadio()
{
  Serial.print("Taxa de dados \t");
  Serial.println(radio.getDataRate());
  Serial.print("Nível do PA \t");
  Serial.println(radio.getPALevel());
  Serial.print("Comprimento do CRC \t");
  Serial.println(radio.getCRCLength());
}

void printRaw(packet dados)
{
  Serial.print("\nAcel. e Gyro.:");
  Serial.print("\t");  Serial.print(dados.tempo);
  for (byte i = 0; i < 3; i++)
  {
    Serial.print("\t");  Serial.print(dados.ac[i]); //Mostra os valores de vel ang na serial
  }
  for (byte i = 0; i < 3; i++)
  {
    Serial.print("\t");  Serial.print(dados.gyr[i]); //Mostra os valores de vel ang na serial
  }
  Serial.print("\nMag e outros:");
  for (byte i = 0; i < 3; i++)
  {
    Serial.print("\t");  Serial.print(dados.mag[i]); //Mostra os valores de vel ang na serial
  }
  Serial.print("\t");  Serial.print(dados.temp);
  Serial.print("\t");  Serial.print(dados.pres);
  Serial.print("\n");
}

void setup()
{
  pinMode(LED, OUTPUT);
  digitalWrite(LED, !LOW); //Inicializa o led desligado, pois nos blue pill ele liga com LOW
  Serial.begin(115200);  //Inicializa a serial
  while (!Serial);
  Serial.println("Inicializando NRF24L01");
  if (!radio.begin())
  {
    while (1)
    {
      Serial.println(F("Falha na fiação ou no NRF24L01!"));
      digitalWrite(LED, HIGH);
      delay(100);
      digitalWrite(LED, LOW);
      delay(100);
    }
  }
  else
    Serial.println(F("NRF24L01 funcinando corretamente"));
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(0, pipe);  //Entra em modo de recepcao
  radio.startListening();
  //debugRadio();
}

void loop()
{
  data_t convertidos;
  if (radio.available()) //Verifica se ha sinal de radio
  {
    digitalWrite(LED, LOW);
    radio.read(&recebidos, sizeof(recebidos));
    Serial.print("\nConvertidos:");
    digitalWrite(LED, HIGH);
    //printRaw(recebidos);
    
    processData(recebidos, &convertidos);
    printData(&Serial, &convertidos);
  }
  else
    //Serial.print(".");
  delay(1);
}
