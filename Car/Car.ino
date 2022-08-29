#include <TimerOne.h>
#include<Servo.h>
Servo my;
int ENC_A =2; //电机的编码器A端
int ENC_B =3; //电机的编码器B端
int count =0; //上升沿（脉冲）数量
int counb=0;
int PWM = 6;
int pwmb=5;
int IN1 = 7;
int IN2 = 8;
int in3=8;
//int in4=9;
int rx=A0;    
int ry=A1;
int red=A5;
int green=A4;
int blue=A3;
float err =0,derr=0,dderr=0;
float Kp=0.1,Ki = 0.03,Kd = 0.15; //可能在某些电机上该参数不收敛，此时请自行手动调节
float rpm,rpmb;
float goal =180;
int sw=12;
int pwm;.

void setup()
{
  while(Serial.available())
  {
    char a[8];
    Serial.readBytes(a,7);
  }
  my.attach(9);
  my.write(90);
    Serial.begin(9600);
    pinMode(IN1,OUTPUT);
    pinMode(IN2,OUTPUT);
    pinMode(in3,OUTPUT);
    pinMode(blue,OUTPUT);
    pinMode(red,OUTPUT);
    pinMode(blue,OUTPUT);
    pinMode(PWM,OUTPUT);
    pinMode(ENC_A,INPUT);
    pinMode(ENC_B,INPUT);
    attachInterrupt(0,Code0,CHANGE);
    attachInterrupt(1,Code1,CHANGE);
    //Timer1.initialize(50000);
    //Timer1.attachInterrupt(TimerIsr);
}

void loop() 
{
while(Serial.available())
{
   char b[8]={0,0,0,0,0,0,0};
   Serial.readBytes(b,7);
  int retu[2]={0,0};
  for (int j=0;j<7;j++)
  {
    if(b[j]!=':')
    {
    if(j<3)
    {
      retu[0]+=(b[j]-'0')*pow(10,(2-j));
    }
    else  if(j>3)
   {
    retu[1]+=(b[j]-'0')*pow(10,6-j);
   }
  }
  }
  Serial.println(b);
  Serial.print(retu[0]);
  Serial.println(retu[1]);
  if(retu[1]>180)
    {my.write(50);
    digitalWrite(blue,LOW);
    digitalWrite(red,LOW);
    digitalWrite(green,LOW);
    digitalWrite(green,HIGH);}
    else if (retu[1]<110)
    {my.write(130);
    digitalWrite(blue,LOW);
    digitalWrite(red,LOW);
    digitalWrite(green,LOW);
    digitalWrite(blue,HIGH);
    }
    else my.write(90);
    rpm = count*60.0/13.0/0.05/20/4;
    count =0;
    goal=(retu[0]-128)*3;
    if(goal<-50)
    {digitalWrite(blue,LOW);
    digitalWrite(red,LOW);
    digitalWrite(green,LOW);digitalWrite(red,HIGH);}
    pwm = PID(goal,rpm);
   
    if(abs(pwm)>255) //用于防止pwm超过0-255范围，可以去掉
    {
        if(pwm<0){pwm = -255;}
        else{pwm = 255;}
    }
    if(pwm <0) //用于处理目标转速为负数时情况
    {
        digitalWrite(IN1,LOW); //反转
        digitalWrite(IN2,HIGH);
        pwm = -pwm;
        //digitalWrite(red,HIGH);
    }
    else
    {
        digitalWrite(IN1,HIGH); //正转
        digitalWrite(IN2,LOW);
        
    }
//    Serial.print("goal:");
//    Serial.print(goal);
//    Serial.print(" rpm:");
//    Serial.println(rpm);
    analogWrite(PWM,pwm);
//    delay(50);
//    
}
    
}
float err_l=0;
int PID(float goal,float now)
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
 void TimerIsr1()
{
  
}
void TimerIsr()
{
    rpm = count*60.0/13.0/0.05/20/4;
    count =0;
    goal=(analogRead(rx)-512)/2;
    pwm = PID(goal,rpm);
    int bb=analogRead(ry);
  if(bb>700)
 {my.write(60);
    delay(500);}
    if (bb<300)
    {my.write(120);
    delay(500);} 
    if(abs(pwm)>255) //用于防止pwm超过0-255范围，可以去掉
    {
        if(pwm<0){pwm = -255;}
        else{pwm = 255;}
    }
    if(pwm <0) //用于处理目标转速为负数时情况
    {
        digitalWrite(IN1,LOW); //反转
        digitalWrite(IN2,HIGH);
        pwm = -pwm;
    }
    else
    {
        digitalWrite(IN1,HIGH); //正转
        digitalWrite(IN2,LOW);
        
    }
    analogWrite(PWM,pwm);
    analogWrite(pwmb,(analogRead(ry)-512)/3);
   // analogWrite(pwm1,-pwm);
}
