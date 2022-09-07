#include <GreyScaleSensor.h>
#include <TB6612.h>

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
GREYSCALESENSOR sensor = GREYSCALESENSOR(FRONT_LEFT_SENSOR, FRONT_MIDDLE_SENSOR, FRONT_RIGHT_SENSOR, 500, BLACK, WHITE);

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

void setup(){
    ;
}

int main(){
    cruise_slowly_strictly();
}
