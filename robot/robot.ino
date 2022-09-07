#include <TB6612.h>
#include <GreyScaleSensor.h>
#include <NewPing.h>
#include <CK008.h>
#include <RGB.h>
#include <WhiteScaleSensor.h>
#include <Servo.h>
#include <ColorDetector.h>

#define S0 (49)
#define S1 (51)
#define S2 (45)
#define S3 (43)
#define OUT (47)
#define TURNRIGHT (1)
#define TURNBACK  (2)

#define BLACK (1)
#define WHITE (0)

#define LINE BLACK
#define ENV WHITE

#define speedrate (0)

#define WHITE_SENSOR_BOUND (300)
#define EDGE_PIN (A3)
#define TRIGGER_PIN  (28)
#define ECHO_PIN     (30)
#define MAX_DISTANCE (100)
#define CK008_PIN (38)
#define FRONT_LEFT_SENSOR (A0)
#define FRONT_MIDDLE_SENSOR (A1)
#define FRONT_RIGHT_SENSOR (A2)
#define IS_OBSTACLE (cornerCount % 3 == 2 && sonar_able && sonar.ping_cm() > 0 && sonar.ping_cm() > 0)


TB6612 motor = TB6612(12, 11, 13, 9, 10, 8, 7);
RGB rgb(36, 32, 34);
GREYSCALESENSOR sensor = GREYSCALESENSOR(FRONT_LEFT_SENSOR, FRONT_MIDDLE_SENSOR, FRONT_RIGHT_SENSOR, 500, BLACK, WHITE);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
CK008 ck008(CK008_PIN);
WhiteScaleSensor edgeSensor(EDGE_PIN, BLACK, WHITE, WHITE_SENSOR_BOUND);
Servo servo_lowerArm, servo_middleArm, servo_upperArm, servo_hand;
Servo servo_roboticArm, servo_storageBox;
ColorDetector cd(S0, S1, S2, S3, OUT);

#define delta (0)

typedef struct armmm{
    int roboticArm;
    int lowerArm;
    int middleArm;
    int upperArm;
} armstatus;

armstatus back_up = {5,85,53,20}, back_down = {5,110,53,20}, forward_up = {135,85,53,20}, forward_down = {135,115,53,20};
armstatus leftback_up = {5,85,60,25}, leftback_down = {5,110,60,25}, leftforward_up = {150,85,50,40}, leftforward_down = {150,125,50,40};
armstatus rightback_up = {5,85,60,25}, rightback_down = {5,110,60,25}, rightforward_up = {110,85,50,40}, rightforward_down = {110,125,50,40};

// armstatus _back_up = {5,85,53,20}, _back_down = {5,110,53,20}, _forward_up = {140 + delta,85,53,20}, _forward_down = {140 + delta,115,53,20};
// armstatus _leftback_up = {5,85,60,25}, _leftback_down = {5,110,60,25}, _leftforward_up = {165 + delta,85,50,40}, _leftforward_down = {165 + delta,125,50,40};
// armstatus _rightback_up = {5,85,60,25}, _rightback_down = {5,110,60,25}, _rightforward_up = {110 + delta,85,50,40}, _rightforward_down = {110 + delta,125,50,40};
armstatus _back_up = {5,85,53,20}, _back_down = {5,110,60,30}, _forward_up = {140 + delta - 2,85,53,20}, _forward_down = {140 + delta - 2,115,53,20};
armstatus _leftback_up = {5,85,60,25}, _leftback_down = {5,110,65,30}, _leftforward_up = {165 + delta + 1,85,53,40}, _leftforward_down = {165 + delta + 1,125,53,40};
armstatus _rightback_up = {5,85,60,25}, _rightback_down = {5,110,65,30}, _rightforward_up = {110 + delta,85,52,37}, _rightforward_down = {110 + delta,125,52,37};

armstatus forward_down_low = {135,85,105,35}, forward_up_low = {135,55,105,35}, midblock_throw = {60,55,105,35};
armstatus leftforward_up_low = {175,70,65,25}, leftforward_down_low = {175,130,65,25}, block_throw = {60,70,65,25};
armstatus rightforward_up_low = {95,70,65,20}, rightforward_down_low = {95,120,65,20};
// armstatus _back_up = {5,85,53,20}, _back_down = {5,110,53,20}, _forward_up = {140 + delta,85,53,20}, _forward_down = {140 + delta,115,53,20};
// armstatus _leftback_up = {5,85,60,25}, _leftback_down = {5,110,60,25}, _leftforward_up = {165 + delta,85,50,40}, _leftforward_down = {165 + delta,125,50,40};
// armstatus _rightback_up = {5,85,60,25}, _rightback_down = {5,110,60,25}, _rightforward_up = {110 + delta,85,50,40}, _rightforward_down = {110 + delta,125,50,40};


typedef struct hooole{
    int angle = 0;
} Hole;

#define angle1st (5)
#define angle2nd (95)
#define angle3rd (180)

Hole bluehole, redhole, greenhole;

int cornerCount;
unsigned long start_time;
bool sprinted = false, sprinted2 = false;
bool round2 = false;
bool sonar_able = true;

enum RobotMode{
    cruising = 0,
    grasping,
    placing,
} robotmode;

void setup(){
    cornerCount = 1;
    setup_servos();
    rotate_to(angle1st, &servo_storageBox);
    rotate_arm(back_up, 1);
    rotate_arm(back_down, 1);
    while (1){
        if (ck008.detect() == TOUCHED){
            break;
            delay(20);
        }
    }

    start_time = millis();
    rgb.white();
    robotmode = cruising;
}

void loop(){
    if (!sprinted && millis() - start_time > 3400){
        rgb.green();
        force_cruise(800, sprint);
        sprinted = true;
        rgb.white();
    }
    if (round2 && !sprinted2 && millis() - start_time > 3000){
        rgb.green();
        force_cruise(950, sprint);
        sprinted2 = true;
        rgb.white();
    }
    if (edgeSensor.detect() == LINE && sprinted){
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
        force_cruise(1550, cruise_slowly_strictly);
        //直接改成读秒orz
        // motor.runright(55);
        // motor.runleft(50);
        // delay(750);
        // motor.runright(50);
        // motor.runleft(50);
        // delay(250);

        motor.stop();

        rgb.magenta();

        if (robotmode == grasping && !round2){
            edgeSensor.disable();
            block_grabbing();
        }
        else if(robotmode == grasping && round2){
            edgeSensor.disable();
            block_grabbing_down();
        }
        else{
            block_placing();
        }


        motor.runleft(-46);
        motor.runright(-50);
        delay(850);
        turnleft();


        // avoid redundant increment to cornerCount
        // otherwise it will get incremented after the call to turnleft() in the loop()
        rgb.turnoff();
        force_cruise(1150, cruise_strictly);
        rgb.magenta();
        force_cruise(400, cruise_down_hill);
        if (robotmode == placing){
            rgb.white();
            force_cruise(5000, cruise);
        }

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
        force_cruise(2500, cruise);
        // force_cruise_rotate(5, 2500, cruise);
        robotmode = cruising;
        rgb.white();
        edgeSensor.enable();
    }
    else{
        cruise();
        delay(10);
    }
    if (cornerCount == 3){
        start_time = millis();
        rgb.turnoff();
        round2 = true;
        sonar_able = true;
        cornerCount = 0;
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
    sonar_able = false;
    //期望其能够斜着上线，后面直接用循迹
}

void turnright(){
    motor.runright(50);
    motor.runleft(50);
    delay(50);
    motor.brake();
    delay(500);
    motor.runright(-65);
    motor.runleft(50); //40
    bool flag = false;
    while (true){
        if (sensor.judgeM() == ENV){
            flag = true;
            delay(100);
        }
        if (flag && sensor.judgeM() == LINE) break;
    }
    motor.brake();
    delay(500);
}

void turnleft(){
    motor.brake();
    delay(200);
    motor.runright(60);
    motor.runleft(-55); //-50
    bool flag = false;
    while (true){
        if (sensor.judgeM() == ENV){
            flag = true;
            delay(100);
        }
        if (flag && sensor.judgeM() == LINE) break;
    }
    motor.brake();
    delay(100);
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

void cruise_down_hill(){
    int LineError;
    LineError = sensor.detect();
    if (LineError == STRAIGHT){
        motor.runright(10);
        motor.runleft(10);
    }
    else if (LineError == RIGHT){
        motor.runright(-20);
        motor.runleft(10);
    }
    else if (LineError == EXTRARIGHT){
        motor.runright(-40);
        motor.runleft(0);
    }
    else if (LineError == LEFT){
        motor.runright(10);
        motor.runleft(-20);
    }
    else if (LineError == EXTRALEFT){
        motor.runright(0);
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


/** Cruise the specified type for speified time (in millisecond).
 * @param time how long the force cruising procedure takes
 * @param cruise_type a pointer to cruise_type function.
 * Available: cruise, cruise_slowly, sprint, cruise_strictly, cruise_slowly_strictly.
 * @brief calls to delay() contained.
 */
void force_cruise(unsigned long time, void (*cruise_type)()){
    unsigned long current = millis();
    while (millis() - current < time){
        cruise_type();
        delay(10);
    }
}

//================================
// 机械臂部分程序



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
    case 4:

        rotate_to(ARMSTATUS.roboticArm, &servo_roboticArm);
        delay(100 * speedrate);
        rotate_to(ARMSTATUS.upperArm, &servo_upperArm);
        delay(300 * speedrate);
        rotate_to(ARMSTATUS.middleArm, &servo_middleArm);
        delay(300 * speedrate);
        rotate_to(ARMSTATUS.lowerArm, &servo_lowerArm);
        delay(300 * speedrate);
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

    rotate_arm(_rightforward_up, 3);
    rotate_arm(_rightforward_down, 1);
    hand_close();
    rotate_to(angle1st, &servo_storageBox);
    // rotate_with_two_servos(5, &servo_storageBox, 95, &servo_hand);
    rotate_arm(_rightforward_up, 3);
    rotate_arm(rightback_up, 2);
    rotate_arm(rightback_down, 1);
    hand_open();
    rotate_arm(back_up, 1);//grab right block


    rotate_arm(_forward_up, 2);
    hand_open();
    rotate_arm(_forward_down, 1);
    hand_close();
    rotate_to(angle2nd, &servo_storageBox);
    rotate_arm(_forward_up, 1);
    rotate_arm(back_up, 3);
    rotate_arm(back_down, 1);
    hand_open();
    rotate_arm(back_up, 1);//grab middle block

    record(cd.color(), angle1st);
    test_light(cd.color());
    delay(500);
    // TODO 数字需要修改

    rotate_arm(_leftforward_up, 3);
    rotate_arm(_leftforward_down, 1);
    hand_close();
    rotate_to(angle3rd, &servo_storageBox);
    rotate_arm(_leftforward_up, 3);
    rotate_arm(leftback_up, 2);
    rotate_arm(leftback_down, 1);
    hand_open();
    rotate_arm(back_up, 1);//grab left block


    record(cd.color(), angle2nd);
    test_light(cd.color());
    delay(500);

    rotate_to(angle1st, &servo_storageBox);
    delay(1000);

    record(cd.color(), angle3rd);
    test_light(cd.color());

    delay(300);
    rgb.turnoff();
}

void block_placing(){
    // 上面一排的顺序：蓝红绿
    hand_open();
    rotate_arm(back_up, 1);
    if (bluehole.angle != 0){
        rotate_to(bluehole.angle, &servo_storageBox);
        rotate_arm(_leftback_down, 1);
        hand_close();
        rotate_arm(_leftback_up, 1);
        rotate_arm(_leftforward_up, 2);
        rotate_arm(_leftforward_down, 1);
        hand_open();
        rotate_arm(_forward_up, 3);
    }

    //place right block

    if (redhole.angle != 0){
        rotate_to(redhole.angle, &servo_storageBox);
        rotate_arm(_back_down, 1);
        hand_close();
        rotate_arm(_back_up, 1);
        rotate_arm(_forward_up, 2);
        rotate_arm(_forward_down, 1);
        hand_open();
        rotate_arm(_forward_up, 1);
        rotate_arm(_back_up, 3);
    }//place middle block

    if (greenhole.angle != 0){
        rotate_to(greenhole.angle, &servo_storageBox);
        rotate_arm(_rightback_down, 1);
        hand_close();
        rotate_arm(_rightback_up, 1);
        rotate_arm(_rightforward_up, 2);
        rotate_arm(_rightforward_down, 1);
        hand_open();
        rotate_arm(_rightforward_up, 3);
        rotate_arm(_back_up, 2);
    }

    rotate_arm(back_down, 1);
}
/**
 * @brief BLUE.
 */

void block_grabbing_down(){
    hand_open();
    rotate_arm(rightforward_up_low, 2);
    rotate_arm(rightforward_down_low, 1);
    hand_close();
    rotate_arm(rightforward_up_low, 1);
    delay(2000);
    rotate_arm(block_throw, 1);
    hand_open();

    rotate_arm(forward_up_low, 2);
    rotate_arm(forward_down_low, 1);
    hand_close();
    rotate_arm(forward_up_low, 1);
    delay(1500);
    rotate_arm(midblock_throw, 1);
    hand_open();

    rotate_arm(leftforward_up_low, 2);
    rotate_arm(leftforward_down_low, 1);
    hand_close();
    rotate_arm(leftforward_up_low, 1);
    delay(1500);
    rotate_arm(block_throw, 1);
    hand_open();

    rotate_arm(back_up, 3);
    rotate_arm(back_down, 1);
    hand_close();
}

void record(String color, int angle){
    if (color == "red"){
        redhole.angle = angle;
    }
    else if (color == "blue"){
        bluehole.angle = angle;
    }
    else if (color == "green"){
        greenhole.angle = angle;
    }
}

void test_light(String color){
    if (color == "red"){
        rgb.red();
    }
    else if (color == "blue"){
        rgb.blue();
    }
    else if (color == "green"){
        rgb.green();
    }
    else{
        rgb.turnoff();
    }
}

// void place_left_up(){
//     rotate_to(bluehole.angle, &servo_storageBox);
//     rotate_arm(back_down, 1);
//     hand_close();
//     rotate_arm(back_up, 1);
//     rotate_arm(leftforward_up, 3);
//     rotate_arm(leftforward_down, 1);
//     hand_open();
//     rotate_arm(leftforward_up, 3);
//     rotate_arm(back_up, 2);//place left block
// }


// /**
//  * @brief RED
//  */
// void place_middle_up(){
//     rotate_to(95, &servo_storageBox);
//     rotate_arm(back_down, 1);
//     hand_close();
//     rotate_arm(back_up, 1);
//     rotate_arm(forward_up, 2);
//     rotate_arm(forward_down, 1);
//     hand_open();
//     rotate_arm(forward_up, 1);
//     rotate_arm(back_up, 3);//place middle block
// }

// /**
//  * @brief GREEN
//  */
// void place_right_up(){
//     rotate_to(180, &servo_storageBox);
//     rotate_arm(back_down, 1);
//     hand_close();
//     rotate_arm(back_up, 1);
//     rotate_arm(rightforward_up, 3);
//     rotate_arm(rightforward_down, 1);
//     hand_open();
//     rotate_arm(rightforward_up, 3);
//     rotate_arm(back_up, 2);//place right block
// }
// /**
//  * @brief 将指定电机转到指定角度，先输入本次指定电机数量，再分别输入：
//  * 电机1的目标角度，电机1指针，电机2的目标角度，电机2指针……
//  *
//  * @param num_of_servos 本次需要使用的电机数量
//  * @param ... 先输入一个角度，再输入一个电机 and so on...
//  */
// void rotate_with_servos(int num_of_servos, ...){
//     va_list valist;
//     va_start(valist, num_of_servos);
//     Servo* servoP;

//     Servo* servos[num_of_servos];
//     int now_angles[num_of_servos];
//     int tar_angles[num_of_servos];
//     double increments[num_of_servos];
//     int increment[num_of_servos];
//     double min_diff = 360.0;
//     for (int i = 0; i < num_of_servos; i++){
//         tar_angles[i] = va_arg(valist, int);
//         servoP = va_arg(valist, Servo*);
//         servos[i] = servoP;
//         now_angles[i] = servoP->read();
//         increments[i] = now_angles[i] - tar_angles[i];
//         if (increments[i] < min_diff){
//             min_diff = increments[i];
//         }
//     }
//     va_end(valist);

//     for (int i = 0; i < num_of_servos; i++){
//         increments[i] /= fabs(min_diff);
//         increment[i] = (int)(increments[i]);
//     }


//     for (int i = 0; i < min_diff; i++){
//         for (int j = 0; j < num_of_servos; j++){
//             now_angles[j] += increment[j];
//             (*(servos[j])).write(now_angles[j]);
//         }
//         delay(15);
//     }
// }

// void rotate_with_two_servos(int angle1, Servo* servo1, int angle2, Servo* servo2){
//     int now_angle1 = servo1->read();
//     int now_angle2 = servo2->read();

//     int increment1 = angle1 > now_angle1 ? 1 : -1;
//     int increment2 = angle2 > now_angle2 ? 1 : -1;

//     while (angle1 != now_angle1 || angle2 != now_angle2){
//         if (angle1 != now_angle1){
//             now_angle1 += increment1;
//             servo1->write(now_angle1);
//         }
//         if (angle2 != now_angle2){
//             now_angle2 += increment2;
//             servo2->write(now_angle2);
//         }
//         delay(15);
//     }
// }


// /**
//  * @brief Rotate the plate while cruising.
//  * calls to delay() included.
//  *
//  * @param target_angle 目标角度
//  * @param time 巡线时间（需大于1.5秒）
//  * @param cruise_type 巡线类型（函数指针）
//  */
// void force_cruise_rotate(int target_angle, int time, void cruise_type()){
//     int current_time = millis();
//     int now_angle = servo_storageBox.read();
//     int increment = now_angle > target_angle ? 1 : -1;
//     while (millis() - current_time < time){
//         cruise_type();
//         if (now_angle != target_angle){
//             now_angle += increment;
//             servo_storageBox.write(now_angle);
//         }
//         delay(15);
//     }
// }
