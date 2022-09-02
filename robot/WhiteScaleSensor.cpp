#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "WhiteScaleSensor.h"
#include "config.h"

#define BOUND 620

WhiteScaleSensor::WhiteScaleSensor(int PIN) {
    pinMode(PIN, INPUT);
    pin = PIN;
}

int WhiteScaleSensor::detect() {
    int val = analogRead(pin);
    if (val > BOUND) {
        return LINE;
    }
    return ENV;
}
