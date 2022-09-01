#include <TB6612.h>
#include <GreyScaleSensor.h>

#define MYPIN 20

#define TURNRIGHT   1
#define TURNBACK    2
#define ENV WHITELINE
#define LINE BLACKLINE

TB6612 motor = TB6612(9, 10, 8, 12, 11, 13, 7);
GREYSCALESENSOR sensor = GREYSCALESENSOR(A2,A1,A0,500,LINE);
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
}

void loop() {
  if(digitalRead(MYPIN) == LINE) //间隔大于多少毫秒才能判断为下一次路口?
    {
        cnt++;
        //右转
        turnright();
        //巡线
        motor.runright(60);
        motor.runleft(60);
        int current_time = millis();
        while((millis() - current_time) > 1000){
          cruise();
          delay(10);
        }  
        //掉头
        turnback();
        //巡线
        motor.runright(60);
        motor.runleft(60);
        current_time = millis();
        while((millis() - current_time) > 1000){
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
