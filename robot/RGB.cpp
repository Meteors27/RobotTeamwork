#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "RGB.h"
#include "config.h"

RGB::RGB(int red, int green, int blue){
    red_pin = red;
    green_pin = green;
    blue_pin = blue;
    pinMode(red_pin, OUTPUT);
    pinMode(green_pin, OUTPUT);
    pinMode(blue_pin, OUTPUT);
}

RGB::set_rgb(int red, int green, int blue){
    analogWrite(red_light_pin, 255 - red_light_value);
    analogWrite(green_light_pin, 255 - green_light_value);
    analogWrite(blue_light_pin, 255 - blue_light_value);
}
