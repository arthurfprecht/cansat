#include <SoftwareSerial.h>
 //5 RX, 6 TX
SoftwareSerial Serial_1 =  SoftwareSerial(5, 6);

void setup() {

  Serial.begin(57600);

  Serial_1.begin(57600);
}

void loop() {

  if (Serial.available()) {      // If anything comes in Serial (USB),

    Serial_1.write(Serial.read());   // read it and send it out Serial_1 (pins 0 & 1)

  }

  if (Serial_1.available()) {     // If anything comes in Serial_1 (pins 0 & 1)

    Serial.write(Serial_1.read());   // read it and send it out Serial (USB)

  }
}
