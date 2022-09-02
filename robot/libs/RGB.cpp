#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "RGB.h"

RGB::RGB(int red, int green, int blue){
    red_pin = red;
    green_pin = green;
    blue_pin = blue;
    pinMode(red_pin, OUTPUT);
    pinMode(green_pin, OUTPUT);
    pinMode(blue_pin, OUTPUT);
}

void RGB::set_rgb(int red, int green, int blue){
    analogWrite(red_pin, 255 - red);
    analogWrite(green_pin, 255 - green);
    analogWrite(blue_pin, 255 - blue);
}

void RGB::twinkle(int red, int green, int blue){
    
}
