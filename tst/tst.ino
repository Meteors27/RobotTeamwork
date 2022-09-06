#include <Servo.h>

Servo servo_hand;


#define speedrate 0.33
void loop(){
    hand_open();
    delay(700);
    hand_close();
    delay(2000);
}


void setup(){
    servo_hand.attach(14);
}

void hand_open(){
    rotate_to(70, &servo_hand);
    delay(300 * speedrate);
    return;
}

void hand_close(){
    rotate_to(95, &servo_hand);
    delay(300 * speedrate);
    return;
}

/**
 * @brief 将servo转到target_angle角度
 * 
 * @param target_angle  目标角度 
 * @param servo  一个Servo*类型，需要转动的舵机指针
 */
void rotate_to(int target_angle, Servo* servo){
    int now_angle;
    now_angle = servo->read();

    if (target_angle < now_angle){
        for (int i = now_angle;i > target_angle;i--){
            servo->write(i);
            delay(15);
        }
    }
    else if (target_angle > now_angle){
        for (int i = now_angle;i < target_angle;i++){
            servo->write(i);
            delay(15);
        }
    }
}
