#ifndef WSS_H
#define WSS_H

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"


#define LINE 0
#define ENV 1

// >= bound indicates black
class WhiteScaleSensor {
    public:
        WhiteScaleSensor(int pin);
        int detect();
    private:
        int pin;
}

#endif