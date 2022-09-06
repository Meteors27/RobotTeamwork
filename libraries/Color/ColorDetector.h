#ifndef COLORDETECOR_H
#define COLORDETECOR_H
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class ColorDetector{
    public:
        ColorDetector();
        unsigned char *readRGB();
        unsigned char *readLux(); 
    private:
        unsigned char Re_buf[11], counter;
        unsigned char sign;
        byte rgb[3];
        void *myserial;
};

#endif