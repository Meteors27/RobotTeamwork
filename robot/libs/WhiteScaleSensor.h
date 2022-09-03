#ifndef WSS_H
#define WSS_H

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif


// >= bound indicates black
class WhiteScaleSensor {
    public:
        WhiteScaleSensor(int pin, int black, int white);

        /* Detect the color below the sensor.
         *@return **LINE / ENV**. LINE indicates black line, ENV indicates white area.
         */
        int detect();

        void enable();
        
        void disable();

        void isable();

    private:
        int pin;
        int black;
        int white;
        bool able;
};

#endif
