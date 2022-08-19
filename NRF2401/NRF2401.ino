#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

#include <SPI.h>

int CE=8;
int CSN=9;
int rx=A0;
int ry=A1;
int sw=4;
RF24 mi(CE,CSN);
const byte address[][6]={"00001","00002"};

void setup() {
  Serial.begin(115200);
  mi.begin();
  pinMode(sw,INPUT_PULLUP);
  mi.openWritingPipe(address[1]);
  mi.setPALevel(RF24_PA_MIN);
  mi.stopListening();
  delay(10);
}

void loop() {
  int data1[3];
  data1[0]=(analogRead(rx)-512)/2;
  data1[1]=(analogRead(ry)-512)/1.5;
  data1[2]=digitalRead(sw);
  Serial.println(data1[0]);
  const byte data[3]={'s','d'};
 mi.write(&data,sizeof(data));
 delay(50);
}
