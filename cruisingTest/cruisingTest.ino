#include <RGB.h>
#include <WhiteScaleSensor.h>

#define MYPIN A8

#define TURNRIGHT   1
#define TURNBACK    2

#define BLACK 1
#define WHITE 0

#define LINE WHITE
#define ENV BLACK

#define TRIGGER_PIN  22
#define ECHO_PIN     23
#define MAX_DISTANCE 80
#define FRONT_LEFT_SENSOR A0
#define FRONT_MIDDLE_SENSOR A1
#define FRONT_RIGHT_SENSOR A2
#define IS_OBSTACLE (cornerCount % 3 == 1 && sonar.ping_cm() <= 50)

#define CK008_PIN 43

WhiteScaleSensor edgeSensor(EDGE_PIN, BLACK, WHITE);
RGB rgb(53, 49, 51);
int val;


void setup(){
  Serial.begin(9600);
  
}

void loop() {
  if (edgeSensor.detect() == ENV){
    Serial.println("env");
    rgb.set_rgb(255,0,0);
  }
  else{
    Serial.println("line");
    rgb.set_rgb(20,0, 255);
  }
  delay(10);
}
