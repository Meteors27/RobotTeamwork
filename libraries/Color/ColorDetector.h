#ifndef COLORDETECOR_H
#define COLORDETECOR_H
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <SoftwareSerial.h>

class ColorDetector{
    public:
        ColorDetector();
        void init();
        unsigned char *readRGB();
        unsigned char *readLux();
        String color(); 
        byte rgb[3];
    private:
        unsigned char Re_buf[11], counter;
        unsigned char sign;
        
};

#endif