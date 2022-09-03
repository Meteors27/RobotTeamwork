#include <TB6612.h>
#include <GreyScaleSensor.h>
#include <NewPing.h>
#include <CK008.h>
#include <RGB.h>
#include <WhiteScaleSensor.h>

#define TURNRIGHT   1
#define TURNBACK    2

#define BLACK 1
#define WHITE 0

#define LINE BLACK
#define ENV WHITE

#define MYPIN A8
#define TRIGGER_PIN  (22)
#define ECHO_PIN     (23)
#define MAX_DISTANCE (80)
#define FRONT_LEFT_SENSOR A0
#define FRONT_MIDDLE_SENSOR A1
#define FRONT_RIGHT_SENSOR A2
#define IS_OBSTACLE (cornerCount % 3 == 2 && sonar.ping_cm() > 0 && sonar.ping_cm() < 80)

#define CK008_PIN 43

TB6612 motor = TB6612(12, 11, 13, 9, 10, 8, 7);
GREYSCALESENSOR sensor = GREYSCALESENSOR(FRONT_LEFT_SENSOR, FRONT_MIDDLE_SENSOR, FRONT_RIGHT_SENSOR, 500, BLACK, WHITE);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
CK008 ck008(CK008_PIN);
RGB rgb(53, 49, 51);
WhiteScaleSensor edgeSensor(MYPIN, BLACK, WHITE);

int cornerCount;

enum RobotMode{
    cruising = 0,
    intersetion,
    obstacle,
    grasping,
    placing,
} robotmode;
// int next = TURNRIGHT;


//TODO
void avoidObstacle(){
    motor.runright(60);
    motor.runleft(90);
    delay(500);
    motor.runright(80);
    motor.runleft(70);
    //期望其能够斜着上线，后面直接用循迹
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

void grasp(){
    if (cornerCount % 3 == 0){

    }
}

void turnright(){
    motor.runright(-40);
    motor.runleft(40);
    int flag = 0;
    while (1){
        if (!flag && sensor.judgeM() == ENV) flag = 1;
        if (flag && sensor.judgeR() == LINE) break;
    }
}


void turnleft(){
    motor.runright(40);
    motor.runleft(-40);
    int flag = 0;
    while (1){
        if (!flag && sensor.judgeR() == ENV) flag = 1;
        if (flag && sensor.judgeM() == LINE) break;
    }
}


void setup(){
    cornerCount = 1;
    int current = millis();
    while (1){
        if (ck008.detect() == TOUCHED || millis() - current > 10000){
            break;
        }
        delay(10);
    }
    pinMode(MYPIN, INPUT);
    rgb.set_rgb(255, 255, 255);
    robotmode = cruising;
}


void loop(){

    if (edgeSensor.detect() == LINE){
        switch (cornerCount % 3){
        case 0:
            robotmode = cruising;
            rgb.set_rgb(255, 255, 255);
            break;
        case 1:
            robotmode = grasping;
            rgb.set_rgb(255, 255, 0);
            break;
        case 2:
            robotmode = placing;
            rgb.set_rgb(0, 255, 255);
            break;
        }
        if (robotmode == grasping || robotmode == placing){
            turnright();
            motor.runright(50);
            motor.runleft(50);
            int current_time = millis();
            while ((millis() - current_time) < 500){
                cruise_slowly();
                delay(10);
            }

            /*
             if (robotmode == grasping) {
                grasp();
                robotmode = grasping;
             }
             else if (robotmode == placing) {
                place();
                robotmode = placing;
             }

            */

            motor.runleft(-50);
            motor.runright(-50);
            current_time = millis();
            while ((millis() - current_time) < 500){
                ;
            }
            turnleft();

            // current_time = millis();
            // //这段唯一的作用就是避免超声波传感器收到干扰
            // while ((millis() - current_time) < 1500){
            //     cruise();
            // }
            robotmode = cruising;
            rgb.set_rgb(255, 255, 255);

        }
        cornerCount++;
    }
    else if (IS_OBSTACLE){
        rgb.set_rgb(255, 0, 0);
        // avoidObstacle过程中亮红灯
        avoidObstacle();
        rgb.set_rgb(0, 0, 255);
        // 调用结束后的巡线先亮蓝灯，出了这个if亮巡线白灯
        int current_time = millis();
        while ((millis() - current_time) < 2500){
            cruise();
            delay(10);
        }
        robotmode = cruising;
        rgb.set_rgb(255, 255, 255);
    }
    else{
        cruise();
        delay(10);
    }
}
// issue: ExtraRight 判断之后转向过程中是不是可能产生意料之外的对ExtraLeft的误判，导致超声波传感器的触发时机不正确？
// 进去取物品再出来，cornerCount加了多少
