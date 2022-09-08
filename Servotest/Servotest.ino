#include <Servo.h>                    
Servo myservo;
void setup(){
//    Serial.(9600);
    myservo.attach(13);
}

void loop(){
    rotate(90);
    while(1);
}

void rotate(int angle){
    int anglex = myservo.read();
    if (angle > anglex){
        for (int i = anglex;i < angle;i++){
            myservo.write(i);
            delay(15);
        }
    }
    else if (angle < anglex){
        for (int i = anglex;i > angle;i--){
            myservo.write(i);
            delay(15);
        }
    }
}
