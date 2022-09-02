#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "GreyScaleSensor.h"

GREYSCALESENSOR::GREYSCALESENSOR(int left, int middle, int right, int compare, int Black, int White){
    LPIN = left;
    MPIN = middle;
    RPIN = right;
    CMP = compare;
    black = Black;
    white = White;
}

int GREYSCALESENSOR::detect(){
    LD = analogRead(LPIN);
    MD = analogRead(MPIN);
    RD = analogRead(RPIN);
    if (LD > CMP && MD <= CMP && RD > CMP){
        return STRAIGHT;
    }
    else if (LD <= CMP && MD <= CMP && RD > CMP){
        return LEFT;
    }
    else if (LD <= CMP && MD > CMP && RD > CMP){
        return EXTRALEFT;
    }
    else if (LD > CMP && MD <= CMP && RD <= CMP){
        return RIGHT;
    }
    else if (LD > CMP && MD > CMP && RD <= CMP){
        return EXTRARIGHT;
    }
    else if (LD <= CMP && MD <= CMP && RD <= CMP){
        return STRAIGHT;
    }
    return STRAIGHT;

}
int GREYSCALESENSOR::read_sensorL(){
    return analogRead(LPIN);
}
int GREYSCALESENSOR::read_sensorM(){
    return analogRead(MPIN);
}
int GREYSCALESENSOR::read_sensorR(){
    return analogRead(RPIN);
}
int GREYSCALESENSOR::judgeR(){
    if (analogRead(RPIN) < CMP) return black;
    else if (analogRead(RPIN) > CMP) return white;
}

int GREYSCALESENSOR::judgeL(){
    if (analogRead(LPIN) < CMP) return black;
    else if (analogRead(LPIN) > CMP) return white;

}
int GREYSCALESENSOR::judgeM(){
    if (analogRead(MPIN) < CMP) return black;
    else if (analogRead(MPIN) > CMP) return white;
}
