#include <TB6612.h>
#include <GreyScaleSensor.h>
#include <NewPing.h>
#include <CK008.h>
#include <RGB.h>
#include <WhiteScaleSensor.h>
#include <Servo.h>

#define TURNRIGHT 1
#define TURNBACK  2

#define BLACK 1
#define WHITE 0

#define LINE BLACK
#define ENV WHITE

#define speedrate 1

#define WHITE_SENSOR_BOUND (330)
#define EDGE_PIN (A3)
#define TRIGGER_PIN  (28)
#define ECHO_PIN     (30)
#define MAX_DISTANCE (100)
#define CK008_PIN (38)
#define FRONT_LEFT_SENSOR (A0)
#define FRONT_MIDDLE_SENSOR (A1)
#define FRONT_RIGHT_SENSOR (A2)
#define IS_OBSTACLE (cornerCount % 3 == 2 && sonar.ping_cm() > 0 && sonar.ping_cm() > 0)


TB6612 motor = TB6612(12, 11, 13, 9, 10, 8, 7);
RGB rgb(36, 32, 34);
GREYSCALESENSOR sensor = GREYSCALESENSOR(FRONT_LEFT_SENSOR, FRONT_MIDDLE_SENSOR, FRONT_RIGHT_SENSOR, 500, BLACK, WHITE);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
CK008 ck008(CK008_PIN);
WhiteScaleSensor edgeSensor(EDGE_PIN, BLACK, WHITE, WHITE_SENSOR_BOUND);
Servo servo_lowerArm, servo_middleArm, servo_upperArm, servo_hand;
Servo servo_roboticArm, servo_storageBox;

typedef struct armmm{
    int roboticArm;
    int lowerArm;
    int middleArm;
    int upperArm;
} armstatus;

armstatus back_up = {5,85,53,20}, back_down = {5,110,53,20}, forward_up = {135,85,53,20}, forward_down = {135,115,53,20};
armstatus leftback_up = {5,85,53,20}, leftback_down = {5,110,53,20}, leftforward_up = {155,110,32,10}, leftforward_down = {155,140,10,0};
armstatus rightback_up = {5,85,53,20}, rightback_down = {5,110,53,20}, rightforward_up = {120,110,32,10}, rightforward_down = {120,140,10,0};

int cnt = 1;

int cornerCount;
int start_time;
int sprinted = false;

enum RobotMode{
    cruising = 0,
    obstacle,
    grasping,
    placing,
} robotmode;

void setup(){
    cornerCount = 1;
    int current = millis();
    while (1){
        if (ck008.detect() == TOUCHED || millis() - current > 10000){
            break;
        }
        delay(10);
    }

    start_time = millis();
    rgb.white();
    robotmode = cruising;
}

void loop(){
    if (!sprinted && millis() - start_time > 3800){
        force_cruise(600, sprint);
        sprinted = true;
    }
    if (edgeSensor.detect() == LINE){
        switch (cornerCount % 3){
        case 0:
            robotmode = cruising;
            rgb.white();
            break;
        case 1:
            robotmode = grasping;
            rgb.yellow();
            break;
        case 2:
            robotmode = placing;
            rgb.cyan();
            break;
        }
        cornerCount++;
    }
    if (robotmode == grasping || robotmode == placing){

        turnright();
        motor.runright(50);
        motor.runleft(50);
        force_cruise(1200, cruise_slowly_strictly);

        motor.stop();
        delay(500);

        // setup_servos();

        rgb.turnoff();
        // while (1){
        //     block_placing();
        // }

        rgb.yellow();

        // while (1){
        //     if (cnt == 1){
        //         block_grabbing();
        //         cnt = 0;
        //     }
        // }
        /*
         if (robotmode == grasping) {
            grasp();
         }
         else {
            place();
         }
        */

        motor.runleft(-50);
        motor.runright(-50);
        delay(500);
        turnleft();


        // avoid redundant increment to cornerCount
        // otherwise it will get intcremented after the call to turnleft() in the loop()
        rgb.turnoff();
        force_cruise(2000, cruise_slowly_strictly);
        robotmode = cruising;
        rgb.white();
    }
    else if (IS_OBSTACLE){
        rgb.red();
        // avoidObstacle过程中亮红灯

        avoidObstacle();
        rgb.blue();
        // 调用结束后的巡线先亮蓝灯，出了这个if亮巡线白灯
        // 在这里加入不带delay的颜色传感器detect并算出需要转的角度
        force_cruise(1000, cruise_strictly);
        //TODO 角度待改动
        force_cruise(2500, cruise);
        // force_cruise_rotate(120, 2500, cruise);
        robotmode = cruising;
        rgb.white();
    }
    else{
        cruise();
        delay(10);
    }

}

void setup_servos(){
    servo_lowerArm.attach(4);
    servo_middleArm.attach(3);
    servo_upperArm.attach(2);
    servo_hand.attach(14);

    servo_roboticArm.attach(5);
    servo_storageBox.attach(6);
}

/**
 * @brief 调用本函数时，机器人向右转弯后向左转弯，走S形曲线回到线上，到线上时，
 * 与线夹角较小，可以直接使用循迹函数直接纠正循迹。
 * @author Kaixuan Wang
 */
void avoidObstacle(){
    motor.runright(60);
    motor.runleft(90);
    delay(700);
    motor.runright(80);
    motor.runleft(74);
    while (sensor.judgeM() != LINE){
        ;
    }
    //期望其能够斜着上线，后面直接用循迹
}

void turnright(){
    motor.runright(-70);
    motor.runleft(40);
    delay(1000);
}

void turnleft(){
    motor.runright(60);
    motor.runleft(-50);
    delay(950);
}

void cruise(){
    int LineError;
    LineError = sensor.detect();
    if (LineError == STRAIGHT){
        motor.runright(90);
        motor.runleft(90);
    }
    else if (LineError == RIGHT){
        motor.runright(30);
        motor.runleft(90);
    }
    else if (LineError == EXTRARIGHT){
        motor.runright(0);
        motor.runleft(90);
    }
    else if (LineError == LEFT){
        motor.runright(90);
        motor.runleft(30);
    }
    else if (LineError == EXTRALEFT){
        motor.runright(90);
        motor.runleft(0);
    }
}

void cruise_strictly(){
    int LineError;
    LineError = sensor.detect();
    if (LineError == STRAIGHT){
        motor.runright(60);
        motor.runleft(60);
    }
    else if (LineError == RIGHT){
        motor.runright(0);
        motor.runleft(60);
    }
    else if (LineError == EXTRARIGHT){
        motor.runright(-10);
        motor.runleft(60);
    }
    else if (LineError == LEFT){
        motor.runright(60);
        motor.runleft(0);
    }
    else if (LineError == EXTRALEFT){
        motor.runright(60);
        motor.runleft(-10);
    }
}

void cruise_slowly(){
    int LineError;
    LineError = sensor.detect();
    if (LineError == STRAIGHT){
        motor.runright(50);
        motor.runleft(50);
    }
    else if (LineError == RIGHT){
        motor.runright(20);
        motor.runleft(50);
    }
    else if (LineError == EXTRARIGHT){
        motor.runright(0);
        motor.runleft(50);
    }
    else if (LineError == LEFT){
        motor.runright(50);
        motor.runleft(20);
    }
    else if (LineError == EXTRALEFT){
        motor.runright(50);
        motor.runleft(0);
    }
}

void cruise_slowly_strictly(){
    int LineError;
    LineError = sensor.detect();
    if (LineError == STRAIGHT){
        motor.runright(40);
        motor.runleft(40);
    }
    else if (LineError == RIGHT){
        motor.runright(0);
        motor.runleft(40);
    }
    else if (LineError == EXTRARIGHT){
        motor.runright(-40);
        motor.runleft(40);
    }
    else if (LineError == LEFT){
        motor.runright(40);
        motor.runleft(0);
    }
    else if (LineError == EXTRALEFT){
        motor.runright(40);
        motor.runleft(-40);
    }
}

void sprint(){
    int LineError;
    LineError = sensor.detect();
    if (LineError == STRAIGHT){
        motor.runright(110);
        motor.runleft(110);
    }
    else if (LineError == RIGHT){
        motor.runright(30);
        motor.runleft(110);
    }
    else if (LineError == EXTRARIGHT){
        motor.runright(0);
        motor.runleft(110);
    }
    else if (LineError == LEFT){
        motor.runright(110);
        motor.runleft(30);
    }
    else if (LineError == EXTRALEFT){
        motor.runright(110);
        motor.runleft(0);
    }
}

void grasp(){
    if (cornerCount % 3 == 0){

    }
}

void place(){

}


/** Cruise the specified type for speified time (in millisecond).
 * @param time how long the force cruising procedure takes
 * @param cruise_type a pointer to cruise_type function.
 * Available: cruise, cruise_slowly, sprint, cruise_strictly, cruise_slowly_strictly.
 * @brief calls to delay() contained.
 */
void force_cruise(int time, void cruise_type()){
    int current = millis();
    while (millis() - current < time){
        cruise_type();
        delay(10);
    }
}

//================================
// 机械臂部分程序

// void block_placing(){
//     rotate_to(3, &servo_roboticArm);
//     delay(200);

//     rotate_to(50, &servo_hand);  //手张开
//     delay(200);

//     rotate_to(110, &servo_lowerArm); //最底下的舵机到位
//     delay(300);
//     rotate_to(53, &servo_middleArm);//第二个舵机到位
//     delay(300);
//     rotate_to(20, &servo_upperArm); //最上面的舵机到位，打下来
//     delay(300);

//     rotate_to(95, &servo_hand); //爪子合拢抓取
//     delay(1000);

//     angle_lowerArm_slow(90);//抬升
//     delay(1000);

//     rotate_to(135, &servo_roboticArm); //转180度
//     delay(1000);

//     rotate_to(110, &servo_lowerArm); //下降，最底下的舵机到位
//     delay(300);

//     rotate_to(50, &servo_hand); //爪子松开，放置
//     delay(200);

//     angle_lowerArm_slow(90);//抬升
//     delay(1000);

//     //下面是抓取
//     rotate_to(110, &servo_lowerArm); //最底下的舵机到位
//     delay(300);

//     rotate_to(95, &servo_hand); //爪子合拢抓取
//     delay(1000);

//     angle_lowerArm_slow(90);//抬升
//     delay(1000);
// }

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

void angle_lowerArm_slow(int angle){
    int now_angle;
    now_angle = servo_lowerArm.read();

    if (angle < now_angle){
        for (int i = now_angle;i > angle;i--){
            servo_lowerArm.write(i);
            delay(30);
        }
    }
    else if (angle > now_angle){
        for (int i = now_angle;i < angle;i++){
            servo_lowerArm.write(i);
            delay(30);
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
    rotate_arm(back_up, 1);

    rotate_arm(leftforward_up, 3);
    rotate_arm(leftforward_down, 1);
    hand_close();
    rotate_arm(leftforward_up, 3);
    rotate_arm(back_up, 2);
    rotate_arm(back_down, 1);
    hand_open();
    rotate_arm(back_up, 1);//grab left block

    rotate_arm(forward_up, 2);
    hand_open();
    rotate_arm(forward_down, 1);
    hand_close();
    rotate_arm(forward_up, 1);
    rotate_arm(back_up, 3);
    rotate_arm(back_down, 1);
    hand_open();
    rotate_arm(back_up, 1);//grab middle block

    rotate_arm(rightforward_up, 3);
    rotate_arm(rightforward_down, 1);
    hand_close();
    rotate_arm(rightforward_up, 3);
    rotate_arm(back_up, 2);
    rotate_arm(back_down, 1);
    hand_open();
    rotate_arm(back_up, 1);//grab right block
}

void block_placing(){
    rotate_arm(back_up, 1);

    rotate_arm(back_down, 1);
    hand_close();
    rotate_arm(back_up, 1);
    rotate_arm(rightforward_up, 3);
    rotate_arm(rightforward_down, 1);
    hand_open();
    rotate_arm(rightforward_up, 3);
    rotate_arm(back_up, 2);//place right block

    rotate_arm(back_down, 1);
    hand_close();
    rotate_arm(back_up, 1);
    rotate_arm(forward_up, 2);
    rotate_arm(forward_down, 1);
    hand_open();
    rotate_arm(forward_up, 1);
    rotate_arm(back_up, 3);//place middle block

    rotate_arm(back_down, 1);
    hand_close();
    rotate_arm(back_up, 1);
    rotate_arm(leftforward_up, 3);
    rotate_arm(leftforward_down, 1);
    hand_open();
    rotate_arm(leftforward_up, 3);
    rotate_arm(back_up, 2);//place left block
}

/**
 * @brief Rotate the plate while cruising.
 * calls to delay() included.
 *
 * @param target_angle 目标角度
 * @param time 巡线时间（需大于1.5秒）
 * @param cruise_type 巡线类型（函数指针）
 */
void force_cruise_rotate(int target_angle, int time, void cruise_type()){
    int current_time = millis();
    int now_angle = servo_storageBox.read();
    int increment = now_angle > target_angle ? 1 : -1;
    while (millis() - current_time < time){
        cruise_type();
        if (now_angle != target_angle){
            now_angle += increment;
            servo_storageBox.write(now_angle);
        }
        delay(15);
    }
}
