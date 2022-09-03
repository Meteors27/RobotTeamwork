#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "WhiteScaleSensor.h"

#define BOUND 350

WhiteScaleSensor::WhiteScaleSensor(int PIN, int Black, int White) {
    pinMode(PIN, INPUT);
    pin = PIN;
    black = Black;
    white = White;
    able = true;
}

int WhiteScaleSensor::detect() {
    if (!able) return white;
    int val = analogRead(pin);
    if (val > BOUND) {
        return black;
    }
    return white;
}

void WhiteScaleSensor::enable() {
    able = true;
}

void WhiteScaleSensor::disable() {
    able = false;
    disable_time = millis();
}

int WhiteScaleSensor::get_disable_time() {
    return disable_time;
}

bool WhiteScaleSensor::isable() {
    return able;
}