#include <TB6612.h>
#include <GreyScaleSensor.h>
#include <NewPing.h>
#include <CK008.h>
#include <RGB.h>
#include <WhiteScaleSensor.h>
#include <Servo.h>

#define TURNRIGHT   1
#define TURNBACK    2

#define BLACK 1
#define WHITE 0

#define LINE BLACK
#define ENV WHITE

#define WHITE_SENSOR_BOUND (400)
#define EDGE_PIN (A9)
#define TRIGGER_PIN  (23)
#define ECHO_PIN     (22)
#define MAX_DISTANCE (100)
#define FRONT_LEFT_SENSOR A0
#define FRONT_MIDDLE_SENSOR A1
#define FRONT_RIGHT_SENSOR A2
#define IS_OBSTACLE (cornerCount % 3 == 2 && sonar.ping_cm() > 0 && sonar.ping_cm() > 0)

#define CK008_PIN 43


TB6612 motor = TB6612(12, 11, 13, 9, 10, 8, 7);
GREYSCALESENSOR sensor = GREYSCALESENSOR(FRONT_LEFT_SENSOR, FRONT_MIDDLE_SENSOR, FRONT_RIGHT_SENSOR, 500, BLACK, WHITE);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
CK008 ck008(CK008_PIN);
RGB rgb(37, 33, 35);
WhiteScaleSensor edgeSensor(EDGE_PIN, BLACK, WHITE, WHITE_SENSOR_BOUND);
Servo servo_lowerArm, servo_middleArm, servo_upperArm, servo_hand;
Servo servo_roboticArm, servo_storageBox;

int cornerCount;
int start_time;
int sprinted = false;

enum RobotMode{
    cruising = 0,
    intersetion,
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
    pinMode(EDGE_PIN, INPUT);
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

        setup_servos();

        while (1){
            block_placing();
        }

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
        force_cruise(2000, cruise_slowly);
        robotmode = cruising;
        rgb.white();
    }
    else if (IS_OBSTACLE){
        rgb.red();
        // avoidObstacle过程中亮红灯

        avoidObstacle();
        rgb.blue();
        // 调用结束后的巡线先亮蓝灯，出了这个if亮巡线白灯
        force_cruise(1000, cruise_strictly);
        force_cruise(2500, cruise);
        robotmode = cruising;
        rgb.white();
    }
    else{
        cruise();
        delay(10);
    }

}

void setup_servos(){
    servo_lowerArm.attach(17);
    servo_middleArm.attach(16);
    servo_upperArm.attach(15);
    servo_hand.attach(14);

    servo_roboticArm.attach(18);
    servo_storageBox.attach(20);
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

void block_placing(){
    rotate_to(177, &servo_roboticArm);
    delay(200);

    rotate_to(50, &servo_hand);  //手张开
    delay(200);

    rotate_to(135, &servo_lowerArm); //最底下的舵机到位
    delay(300);
    rotate_to(53, &servo_middleArm);//第二个舵机到位
    delay(300);
    rotate_to(20, &servo_upperArm); //最上面的舵机到位，打下来
    delay(300);

    rotate_to(95, &servo_hand); //爪子合拢抓取
    delay(1000);

    angle_lowerArm_slow(110);//抬升
    delay(1000);

    rotate_to(45, &servo_roboticArm); //转180度
    delay(1000);

    rotate_to(135, &servo_lowerArm); //下降，最底下的舵机到位
    delay(300);

    rotate_to(50, &servo_hand); //爪子松开，放置
    delay(200);

    angle_lowerArm_slow(110);//抬升
    delay(1000);

    //下面是抓取
    rotate_to(135, &servo_lowerArm); //最底下的舵机到位
    delay(300);

    rotate_to(95, &servo_hand); //爪子合拢抓取
    delay(1000);

    angle_lowerArm_slow(110);//抬升
    delay(1000);
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
            servo_hand.write(i);
            delay(15);
        }
    }
    else if (target_angle > now_angle){
        for (int i = now_angle;i < target_angle;i++){
            servo_hand.write(i);
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
