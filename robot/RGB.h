#ifndef RGB_H
#define RGB_H

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

class RGB{
    public:
        RGB(int red, int green, int blue);

        /* Set the RGB value. */
        set_rgb(int red, int green, int blue);
    
    private:
        int red_pin;
        int blue_pin;
        int green_pin;
};

#endif
