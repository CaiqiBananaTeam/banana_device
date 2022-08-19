#include <U8glib.h>
#include <SoftwareSerial.h>
#include <TimerOne.h>
#include<Servo.h>
U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);
Servo my;
SoftwareSerial mySerial(8, 9); // RX, TX
int IN1=10;
int IN2=11;
int IN3=12;
int IN4=13;
int PWM1=6;
int PWM2=5;
int ENC_A=2;
int ENC_B=3;
//int pww=5;
int count=0;
float pwm;
float err =0,derr=0,dderr=0;
float Kp=0.1,Ki = 0.03,Kd = 0.15; //可能在某些电机上该参数不收敛，此时请自行手动调节
void setup() {
  digitalWrite(IN1,HIGH); //正转//失速的原因就是因为这个..666666直接设置为只有正传就可以了
  digitalWrite(IN2,LOW);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(PWM1,OUTPUT);
  pinMode(PWM2,OUTPUT);
  my.attach(5);
  my.write(0);
  Serial.begin(9600);
  mySerial.begin(9600);
  delay(1000);
  Serial.println("Goodnight  moon2");
  mySerial.println("AT+GMR");
  delay(1000);
  mySerial.println("AT+CIPMUX=1");
  delay(1000);
  mySerial.println("AT+CIPSERVER=1,8081");
  attachInterrupt(0,Code0,CHANGE);
  attachInterrupt(1,Code1,CHANGE);
}

//int* change(String str) {
//  int len = str.length();
//  int j = 0, flag = 0;
//  int ret[3] = { 0 };
//  if(str[0]=='+'){
//  for (int i = len - 1; j < 3; i--)
//  {
//    
//    char now = str[i];
//    if (now == ':')
//    {
//      flag = 0; j++;
//    }
//    else
//    {
//      ret[j] += (now - '0') * pow(10, flag++);
//    }
//  }
//  return ret;}
//  else return 1;
//}
int ret[3] = { 0 };
void loop() 
{
  my.write(0);
  while (mySerial.available()) {//所有行为建立在软串口开启的状态
     String str=mySerial.readString();
    //Serial.println(str);
     int a=str.length()-1;
     //int *ret;
     if(str[3]=='I')
     { int len = str.length();//截取wifi发送的信息
      int j = 0, flag = 0;
      //int ret[3] = { 0 };
      char led[10]={0};
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
         //Serial.println(ret[0]);
          Serial.println(ret[0]);
         u8g.firstPage();
        do{
            char xx[4]={'a','b','c'};
            
          u8g.setFont(u8g_font_unifont);
          u8g.drawStr( 30, 22, led);//打印函数
        }while(u8g.nextPage());
    int tim[3]={0};
    
   // analogWrite(PWM2,20*pwm);
   // delay(50);
   
    }//wifi信息截取大括号
  }//软串口大括号
  if(ret[0]!=0){
  unsigned long timenow=millis();
  float rpm = count*60.0/13.0/0.05/20/2;
  double time1[3]={0};
  time1[0]=ret[0]*1000.0;
  time1[1]=ret[1]*1000.0;
  time1[2]=ret[2]*1000.0;//*1000/60;
  while(millis()-timenow<time1[0]){
    Serial.println("n0");
    rpm = count*60.0/13.0/0.05/20/2;
    count=0;
    float goal=10;
     pwm=PID(goal,rpm);
    if(abs(pwm)>255) //用于防止pwm超过0-255范围，可以去掉
    {
        if(pwm<0){pwm = -255;}
        else{pwm = 255;}
    }
        
//    digitalWrite(IN3,HIGH); //正转
//    digitalWrite(IN4,LOW);
//    Serial.print(count);
//    Serial.print("goal:");
//    Serial.print(goal);
//    Serial.print(" rpm:");
//    Serial.println(rpm);
    analogWrite(PWM1,pwm);
  }
  Serial.println("666");
  delay(2000);
  my.write(15);
//  int next1=0;
//  {剪线部分完成赋值为1；
//  while(next1==1&&millis()-timenow<time1[1]){}
//  int next2=0;
//  //剪线完成后赋值为1；
//  while(next2==1&&millis()-timenow<time1[2]){}
//  delay(1000);
//  //前面都是剥线，现在需要剪线，舵机转移一次，剪一次，完成后在转回来//
  timenow=millis();
  while(millis()-timenow<time1[2]){
    Serial.println("n1");
    rpm = count*60.0/13.0/0.05/20/2;
    count=0;
    float goal=10;
     pwm=PID(goal,rpm);
    if(abs(pwm)>255) //用于防止pwm超过0-255范围，可以去掉
    {
        if(pwm<0){pwm = -255;}
        else{pwm = 255;}
    }
        
//    digitalWrite(IN3,HIGH); //正转
//    digitalWrite(IN4,LOW);
//    Serial.print(count);
//    Serial.print("goal:");
//    Serial.print(goal);
//    Serial.print(" rpm:");
//    Serial.println(rpm);
    analogWrite(PWM1,pwm);
  }
  delay(2000);
  my.write(0);
  } 
}//loop大括号
float err_l=0;
float PID(float goal,float now)
{
    //dderr = goal - now - err - derr;
    //derr = goal - now - err;
    float err1 = goal - now;
    err+=err1;
    
    float PWM=Kp*err1+Ki*err+Kd*(err1-err_l);
    err_l=err1;
    return PWM;
}

void Code0()
{
    if(digitalRead(ENC_A)  == LOW) //判断A为下降沿
    {
        if(digitalRead(ENC_B) == LOW)//正转
        {
            count ++;
        }
        if(digitalRead(ENC_B) == HIGH)//反转
        {
            count --;
        }
    }
 if(digitalRead(ENC_A)==HIGH)//上升
    {
      if(digitalRead(ENC_B)==LOW)//翻转
      {
        count--;
      }
      if(digitalRead(ENC_B)==HIGH)//正向
      {
        count++;
      }
     }
}
void Code1()
{
 if(digitalRead(ENC_B)  == LOW) //判断B为下降沿
    {
        if(digitalRead(ENC_A) == LOW)//反转
        {
            count --;
        }
        if(digitalRead(ENC_A) == HIGH)//正传
        {
            count ++;
        }
    }
  if(digitalRead(ENC_B)==HIGH)//上升
    {
      if(digitalRead(ENC_A)==LOW)//正传
      {
        count++;
      }
      if(digitalRead(ENC_A)==HIGH)//反转
      {
        count--;
      }
     } 
 }   
