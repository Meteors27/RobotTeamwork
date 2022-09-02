#include <Servo.h>                    
Servo myservo;
void setup(){
//    Serial.begin(9600);
    myservo.attach(13);
}

void loop(){
    rotate(0);
    delay(500);
    rotate(120);
//    Serial.println("+");
  /* Rotating a positive angle indicates the hand open. */
    delay(2000);
    rotate(-120);
//    Serial.println("-");
    delay(1000);
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
