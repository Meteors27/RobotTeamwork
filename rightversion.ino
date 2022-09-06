#include <Servo.h>  
#include <CK008.h>     
#include <stdarg.h>
#include <math.h>          
Servo servo_lowerArm, servo_middleArm, servo_upperArm, servo_hand;
Servo servo_roboticArm, servo_storageBox;
int cnt = 1;

#define CK008_PIN 38
#define STORAGEBOX_1 5 
#define STORAGEBOX_2 90
#define STORAGEBOX_3 180
#define speedrate 0.1

CK008 ck008(CK008_PIN);

typedef struct armmm{
    int roboticArm;
    int lowerArm;
    int middleArm;
    int upperArm;
}armstatus;

armstatus back_up = {5,85,53,20}, back_down = {5,110,53,20}, forward_up = {135,85,53,20}, forward_down = {135,115,53,20};
armstatus leftback_up = {5,85,53,20}, leftback_down = {5,110,53,20}, leftforward_up = {165,110,32,10}, leftforward_down = {165,140,10,0};
armstatus rightback_up = {5,85,53,20}, rightback_down = {5,110,53,20}, rightforward_up = {110,100,32,10}, rightforward_down = {110,140,10,0};

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

void rotate_arm(armstatus ARMSTATUS, int rotate_mode){
    switch (rotate_mode){
    case 1:
        rotate_to(ARMSTATUS.middleArm, &servo_middleArm);
        delay(300 * speedrate);
        rotate_to(ARMSTATUS.upperArm, &servo_upperArm);
        delay(300 * speedrate);


        rotate_to(ARMSTATUS.roboticArm, &servo_roboticArm);
        delay(100 * speedrate);
        //新增并行
        // rotate_with_two_servos(ARMSTATUS.roboticArm, &servo_roboticArm, ARMSTATUS.storageBoxz, &servo_storageBox);
        // delay(100 * speedrate);

        rotate_to(ARMSTATUS.lowerArm, &servo_lowerArm);
        delay(300 * speedrate);
        break;
    case 2:

        rotate_to(ARMSTATUS.roboticArm, &servo_roboticArm);
        delay(100 * speedrate);
        rotate_to(ARMSTATUS.lowerArm, &servo_lowerArm);
        delay(300 * speedrate);
        rotate_to(ARMSTATUS.middleArm, &servo_middleArm);
        delay(300 * speedrate);
        rotate_to(ARMSTATUS.upperArm, &servo_upperArm);
        delay(300 * speedrate);
        break;
    case 3:
        rotate_to(ARMSTATUS.lowerArm, &servo_lowerArm);
        delay(300 * speedrate);
        rotate_to(ARMSTATUS.upperArm, &servo_upperArm);
        delay(300 * speedrate);
        rotate_to(ARMSTATUS.middleArm, &servo_middleArm);
        delay(300 * speedrate);
        rotate_to(ARMSTATUS.roboticArm, &servo_roboticArm);
        delay(100 * speedrate);
        break;
    }

    return;
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

void block_grabbing(){
    hand_open();
    rotate_to(5, &servo_storageBox);
    rotate_arm(back_up, 1);

    rotate_arm(rightforward_up, 3);
    rotate_arm(rightforward_down, 1);
    hand_close();
    rotate_to(5, &servo_storageBox);
    // rotate_with_two_servos(5, &servo_storageBox, 95, &servo_hand);
    rotate_arm(rightforward_up, 3);
    rotate_arm(back_up, 2);
    rotate_arm(back_down, 1);
    hand_open();
    rotate_arm(back_up, 1);//grab right block

    rotate_arm(forward_up, 2);
    hand_open();
    rotate_arm(forward_down, 1);
    hand_close();
    rotate_to(90, &servo_storageBox);
    rotate_arm(forward_up, 1);
    rotate_arm(back_up, 3);
    rotate_arm(back_down, 1);
    hand_open();
    rotate_arm(back_up, 1);//grab middle block

    rotate_arm(leftforward_up, 3);
    rotate_arm(leftforward_down, 1);
    hand_close();
    rotate_to(175, &servo_storageBox);
    rotate_arm(leftforward_up, 3);
    rotate_arm(back_up, 2);
    rotate_arm(back_down, 1);
    hand_open();
    rotate_arm(back_up, 1);//grab left block

}

void block_placing(){
    rotate_arm(back_up, 1);
    hand_open();

    rotate_to(5, &servo_storageBox);
    rotate_arm(back_down, 1);
    hand_close();
    rotate_arm(back_up, 1);
    rotate_arm(leftforward_up, 3);
    rotate_arm(leftforward_down, 1);
    hand_open();
    rotate_arm(leftforward_up, 3);
    rotate_arm(back_up, 2);//place left block

    rotate_to(90, &servo_storageBox);
    rotate_arm(back_down, 1);
    hand_close();
    rotate_arm(back_up, 1);
    rotate_arm(forward_up, 2);
    rotate_arm(forward_down, 1);
    hand_open();
    rotate_arm(forward_up, 1);
    rotate_arm(back_up, 3);//place middle block

    rotate_to(175, &servo_storageBox);
    rotate_arm(back_down, 1);
    hand_close();
    rotate_arm(back_up, 1);
    rotate_arm(rightforward_up, 3);
    rotate_arm(rightforward_down, 1);
    hand_open();
    rotate_arm(rightforward_up, 3);
    rotate_arm(back_up, 2);//place right block

}

void setup(){
    servo_lowerArm.attach(4);
    servo_middleArm.attach(3);
    servo_upperArm.attach(2);
    servo_hand.attach(14);

    servo_roboticArm.attach(5);
    servo_storageBox.attach(6);
    while (ck008.detect() != TOUCHED);

}

void loop(){

    if (cnt == 1){
        //block_grabbing();
        block_placing();
        cnt = 0;
    }
}
/**
 * @brief 将指定电机转到指定角度，先输入本次指定电机数量，再分别输入：
 * 电机1的目标角度，电机1指针，电机2的目标角度，电机2指针……
 *
 * @param num_of_servos 本次需要使用的电机数量
 * @param ... 先输入一个角度，再输入一个电机 and so on...
 */
void rotate_with_servos(int num_of_servos, ...){
    va_list valist;
    va_start(valist, num_of_servos);
    Servo* servoP;

    Servo* servos[num_of_servos];
    int now_angles[num_of_servos];
    int tar_angles[num_of_servos];
    double increments[num_of_servos];
    int increment[num_of_servos];
    double min_diff = 360.0;
    for (int i = 0; i < num_of_servos; i++){
        tar_angles[i] = va_arg(valist, int);
        servoP = va_arg(valist, Servo*);
        servos[i] = servoP;
        now_angles[i] = servoP->read();
        increments[i] = now_angles[i] - tar_angles[i];
        if (increments[i] < min_diff){
            min_diff = increments[i];
        }
    }
    va_end(valist);

    for (int i = 0; i < num_of_servos; i++){
        increments[i] /= fabs(min_diff);
        increment[i] = (int)(increments[i]);
    }


    for (int i = 0; i < min_diff; i++){
        for (int j = 0; j < num_of_servos; j++){
            now_angles[j] += increment[j];
            (*(servos[j])).write(now_angles[j]);
        }
        delay(15);
    }
}

void rotate_with_two_servos(int angle1, Servo* servo1, int angle2, Servo* servo2){
    int now_angle1 = servo1->read();
    int now_angle2 = servo2->read();

    int increment1 = angle1 > now_angle1 ? 1 : -1;
    int increment2 = angle2 > now_angle2 ? 1 : -1;

    while (angle1 != now_angle1 || angle2 != now_angle2){
        if (angle1 != now_angle1){
            now_angle1 += increment1;
            servo1->write(now_angle1);
        }
        if (angle2 != now_angle2){
            now_angle2 += increment2;
            servo2->write(now_angle2);
        }
        delay(15);
    }
}
