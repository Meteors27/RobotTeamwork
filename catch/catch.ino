#include<Servo.h>
Servo hand;
Servo upperarm;

void setup()
{
  hand.attach(13);
  upperarm.attach(12);
}

void loop()
{
  upperang(15);
  delay(500);
  
  rotate(60);
  delay(1500);
  rotate(90);
  delay(2000);

  upperang(120);
  delay(5000);
}

void rotate(int angle)
{
  int currentangle;
  currentangle=hand.read();

  if(currentangle>angle)
  {
    for(int i=currentangle;i>angle;i--)
    {
      hand.write(i);
      delay(15);
    }
  }
  else if(currentangle<angle)
  {
    for(int i=currentangle;i<angle;i++)
    {
      hand.write(i);
      delay(15);
    }
  }
}

void upperang(int angle)
{
    int currentangle;
  currentangle=upperarm.read();

  if(currentangle>angle)
  {
    for(int i=currentangle;i>angle;i--)
    {
      upperarm.write(i);
      delay(15);
    }
  }
  else if(currentangle<angle)
  {
    for(int i=currentangle;i<angle;i++)
    {
      upperarm.write(i);
      delay(15);
    }
  }
}
