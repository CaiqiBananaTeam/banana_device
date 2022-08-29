#include <U8glib.h>
#include <SoftwareSerial.h>
#include <TimerOne.h>
#include<Servo.h>
#include <Arduino.h>
#include <U8g2lib.h>
//#include <avr/wdt.h>
//#ifdef U8X8_HAVE_HW_I2C
//#include <Wire.h>
//#endif
U8G2_SH1106_128X32_VISIONOX_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE); 
Servo my;
SoftwareSerial mySerial(8, 9); // RX, TX
const int dir=2;
const int step2=3;
const int dir2=7;
const int stepp=6;
void setup() {
  pinMode(stepp,OUTPUT);
  pinMode(dir,OUTPUT);
  pinMode(dir2,OUTPUT);
  pinMode(step2,OUTPUT);.
  u8g2.begin();
  my.attach(5);
  my.write(8);
  Serial.begin(9600);
  mySerial.begin(9600);
  delay(1000);
  Serial.println("Goodnight  moon2");
  mySerial.println("AT+GMR");
  delay(1000);
  mySerial.println("AT+CIPMUX=1");
  delay(1000);
  mySerial.println("AT+CIPSERVER=1,8081");
  digitalWrite(dir,LOW);
  digitalWrite(dir2,LOW);
//  u8g2.clearBuffer();
//  u8g2.setFont(u8g2_font_6x13_tf);
//    u8g2.drawStr(40,22,"welcome!");
//    u8g2.sendBuffer();
//  u8g2.clearBuffer();
}
int ret[3] = { 0 };//定义为全局变量
 char led[9]={0};
 bool ii=0;
void loop() 
{
//  if(!ii)
//    {u8g2.clearBuffer();
//  u8g2.setFont(u8g2_font_6x13_tf);
//    u8g2.drawStr(40,22,"welcome!");
//    u8g2.sendBuffer();
//    ii++;}
  Serial.println("normal");
  if (mySerial.available()) {//所有行为建立在软串口开启的状态
     String str=mySerial.readString();
     //Serial.println(str);
     int a=str.length()-1;
     //int *ret;
     if(str[3]=='I')
     { int len = str.length();//截取wifi发送的信息
      int j = 0, flag = 0;
      ret[0] = ret[1]=ret[2]=0;//重新设置的时候设为0
      int ledd=0;
      
      for (int i = len - 1; j < 3; i--)//截取其中的字符变为int数组ret
      {
        led[ledd++]=str[len-i+10];
        char now = str[i];
        if (now == ':')
        {
          flag = 0; j++;
        }
        else
        {
          ret[j] += (now - '0') * pow(10, flag++);
        }
      }
      led[9]='\0';
  
  //delay(1000);
    }//wifi信息截取大括号
  }//软串口available大括号
  if(ret[0]>0){//ret[0]代表截取几次
  ret[0]--;
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x13_tf);
  u8g2.drawStr(12,22,led);
  u8g2.setCursor(100,22);
  u8g2.print(ret[0]);
  u8g2.sendBuffer();
    int time1[2]={0};//代表两端长度的映射
    time1[0]=ret[1]*50;//包裹部分
    time1[1]=ret[2]*50;//亮出部分
  for(int x=0;x<time1[1];x++)//第一段，亮出部分
  {
    digitalWrite(stepp,HIGH);
    delay(2);
    digitalWrite(stepp,LOW);
    delay(2);
  }
  delay(500);
//////////////剪一下外皮
  for(int y=0;y<200;y++)
  {
    digitalWrite(step2,HIGH);
    delay(2);
    digitalWrite(step2,LOW);
    delay(2);
  }
 for(int x=0;x<time1[0];x++)//第er段，包裹部分
  {
    digitalWrite(stepp,HIGH);
  delay(2);
  digitalWrite(stepp,LOW);
  delay(2);
  }
  delay(500);
  for(int y=0;y<200;y++)
  {
    digitalWrite(step2,HIGH);
    delay(2);
    digitalWrite(step2,LOW);
    delay(2);
  }
  //////////剪一下外皮第二部分完毕
  for(int x=0;x<time1[1];x++)//第一段，亮出部分
  {
    digitalWrite(stepp,HIGH);
  delay(2);
  digitalWrite(stepp,LOW);
  delay(2);
  }
  delay(500);
  my.write(15);
  delay(1000);
  for(int y=0;y<200;y++)
  {
    digitalWrite(step2,HIGH);
    delay(2);
    digitalWrite(step2,LOW);
    delay(2);
  }
   //////直接剪断，完毕
  delay(500);
  my.write(8);
  }
 // wdt_reset();
}//loop大括号
