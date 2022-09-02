#ifndef GREYSCALESENSOR_H
#define GREYSCALESENSOR_H
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define STRAIGHT    1
#define LEFT        2
#define EXTRALEFT   3
#define RIGHT       4
#define EXTRARIGHT  5
#define CROSS       6

class GREYSCALESENSOR{
public:
    GREYSCALESENSOR(int left, int middle, int right, int compare, int Black, int White);
    int detect();
    int read_sensorL();
    int read_sensorM();
    int read_sensorR();
    int judgeR();
    int judgeL();
    int judgeM();
private:
    int LPIN;
    int MPIN;
    int RPIN;
    int LD;
    int MD;
    int RD;
    int CMP;
    int black;
    int white;
};
#endif