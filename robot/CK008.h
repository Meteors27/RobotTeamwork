#ifndef TOUCHSENSOR_H
#define TOUCHSENSOR_H

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"

class CK008 {
    public:
        /* Initialize the touch sensor. */
        CK008(int sig);

        /* Detect the 
        int detect();
}

#endif