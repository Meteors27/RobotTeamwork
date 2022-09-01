#include <TB6612.h>
#include <GreyScaleSensor.h>
#include <NewPing.h>
#define MYPIN 32

#define TURNRIGHT   1
#define TURNBACK    2
#define ENV WHITELINE
#define LINE BLACKLINE

#define TRIGGER_PIN  22  
#define ECHO_PIN     23
#define MAX_DISTANCE 80

TB6612 motor = TB6612(12, 11, 13, 9, 10, 8, 7);
GREYSCALESENSOR sensor = GREYSCALESENSOR(A0,A1,A2,500,LINE);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
int cnt = 0;
int next = TURNRIGHT;


void cruise()
{
  int LineError;
  LineError = sensor.detect();
  if(LineError == STRAIGHT)
    {
        motor.runright(80);
        motor.runleft(80);
    }
    else if(LineError == RIGHT)
    {
        motor.runright(65);
        motor.runleft(80);
    }
    else if(LineError == EXTRARIGHT)
    {
        motor.runright(20);
        motor.runleft(80);
    }
    else if(LineError == LEFT)
    {
        motor.runright(80);
        motor.runleft(65);
    }
    else if(LineError == EXTRALEFT)
    {
        motor.runright(80);
        motor.runleft(20);
    }
}


void turnright()
{
    motor.runright(-40);
    motor.runleft(40);
    int flag=0;
    while(1)
    {
        if(!flag && sensor.judgeM() == ENV) flag=1;
        if(flag && sensor.judgeR() == LINE) break;
    }
}


void turnback()
{
    motor.runright(-40);
    motor.runleft(40);
    int flag1=0, flag2=0, flag3=0;
    while(1)
    {
        if(sensor.judgeM() == ENV) flag1=1;
        if(flag1 && sensor.judgeM() == LINE) flag2=1;
        if(flag2 && sensor.judgeM() == ENV) flag3=1;
        if(flag3 && sensor.judgeR() == LINE) break;
    }
}


void setup() {
  pinMode(MYPIN, INPUT);
}


void loop() {
  if(cnt%3 == 1 && sonar.ping_cm() <= 50){
      motor.runright(60);
      motor.runleft(90);
      delay(500);
      motor.runright(80);
      motor.runleft(80);
  }
  else if(digitalRead(MYPIN) == ENV) //间隔大于多少毫秒才能判断为下一次路口?
    {
        cnt++;
        //右转
        turnright();
        //巡线
        motor.runright(60);
        motor.runleft(60);
        int current_time = millis();
        while((millis() - current_time) < 1000){
          cruise();
          delay(10);
        }  
        //掉头
        turnback();
        //巡线
        motor.runright(60);
        motor.runleft(60);
        current_time = millis();
        while((millis() - current_time) < 1000){
          cruise();
          delay(10);
        }  
        //右转
        turnright();
    }
    else
    {
        cruise();
        delay(10);
    }
}
