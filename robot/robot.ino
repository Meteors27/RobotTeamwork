#include <TB6612.h>
#include <GreyScaleSensor.h>
#include <NewPing.h>
#include <CK008.h>
#define MYPIN 32

#define TURNRIGHT   1
#define TURNBACK    2
#define ENV WHITELINE
#define LINE BLACKLINE

#define TRIGGER_PIN  22
#define ECHO_PIN     23
#define MAX_DISTANCE 80
#define IS_OBSTACLE (cornerCount % 3 == 1 && sonar.ping_cm() <= 50)



TB6612 motor = TB6612(12, 11, 13, 9, 10, 8, 7);
GREYSCALESENSOR sensor = GREYSCALESENSOR(A0, A1, A2, 500, LINE);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
int cornerCount;
enum RobotMode{
    cruising = 0,
    intersetion,
    obstacle,
    grasping,
    placing,
};
// int next = TURNRIGHT;

void aviodObstacle(){
    motor.runright(60);
    motor.runleft(90);
    delay(500);
    motor.runright(80);
    motor.runleft(80);
}

void cruise(){
    int LineError;
    LineError = sensor.detect();
    if (LineError == STRAIGHT){
        motor.runright(80);
        motor.runleft(80);
    }
    else if (LineError == RIGHT){
        motor.runright(65);
        motor.runleft(80);
    }
    else if (LineError == EXTRARIGHT){
        motor.runright(20);
        motor.runleft(80);
    }
    else if (LineError == LEFT){
        motor.runright(80);
        motor.runleft(65);
    }
    else if (LineError == EXTRALEFT){
        motor.runright(80);
        motor.runleft(20);
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


void turnback(){
    motor.runright(-40);
    motor.runleft(40);
    int flag1 = 0, flag2 = 0, flag3 = 0;
    while (1){
        if (sensor.judgeM() == ENV) flag1 = 1;
        if (flag1 && sensor.judgeM() == LINE) flag2 = 1;
        if (flag2 && sensor.judgeM() == ENV) flag3 = 1;
        if (flag3 && sensor.judgeR() == LINE) break;
    }
}


void setup(){
    cornerCount = 0;
    pinMode(MYPIN, INPUT);
}


void loop(){
    if (IS_OBSTACLE){
        aviodObstacle();
        RobotMode = cruising;
    }
    /* 红外传感器的ENV和LINE常量值是反的
     * 间隔大于多少毫秒才能判断为下一次路口?
     */
    else if (digitalRead(MYPIN) == ENV){
        // Set robot mode.
        switch (cornerCount % 3){
        case 0:
            RobotMode = cruising;
            break;
        case 1:
            RobotMode = grasping;
            break;
        case 2:
            RobotMode = placing;
            break;
        }
        if (RobotMode == cruising){
            //在起点处路口不用转
            turnright();
            //向前靠近桌面
            motor.runright(60);
            motor.runleft(60);
            int current_time = millis();
            while ((millis() - current_time) < 500){
                cruise();
                delay(10);
            }
            //掉头

            /*
             if (RobotMode == grasping) {
                grasp();
                RobotMode = cruising;
             }
             else if (RobotMode == placing) {
                place();
                RobotMode = placing;
             }

            */


            //向后远离桌面
            turnback();
            motor.runright(60);
            motor.runleft(60);
            current_time = millis();
            while ((millis() - current_time) < 500){
                cruise();
                delay(10);
            }
            //右转
            turnright();
        }
        cornerCount++;
    }
    else if (RobotMode == cruising){
        cruise();
        delay(10);
    }
}
// issue: ExtraRight 判断之后转向过程中是不是可能产生意料之外的对ExtraLeft的误判，导致超声波传感器的触发时机不正确？
// 进去取物品再出来，cornerCount加了多少