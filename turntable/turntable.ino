#include <Servo.h>
#define PIN 6
Servo turntable;
void setup()
{
    turntable.attach(PIN);
}

void loop()
{
    rotate(&turntable, 0);
    delay(1000);
    rotate(&turntable, 90);
    delay(1000);
    rotate(&turntable, 180);
    delay(1000);

}

void rotate(Servo *serv, int angle){
    int anglex = serv->read();
    if (angle > anglex){
        for (int i = anglex;i < angle;i++){
            serv->write(i);
            delay(15);
        }
    }
    else if (angle < anglex){
        for (int i = anglex;i > angle;i--){
            serv->write(i);
            delay(15);
        }
    }
}