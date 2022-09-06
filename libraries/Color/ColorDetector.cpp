#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "ColorDetector.h"
#include <SoftwareSerial.h>

ColorDetector::ColorDetector()
{
    Serial1.begin(9600);
    Serial1.write(0xA5);
    Serial1.write(0x81);
    Serial1.write(0x26);
    counter = 0;
    sign = 0;
    myserial = &Serial1;
}
ColorDetector::ColorDetector(int TX, int RX)
{
    SoftwareSerial softSerial(TX, RX);
    softSerial.write(0xA5);
    softSerial.write(0x81);
    softSerial.write(0x26);
    counter = 0;
    sign = 0;
    myserial = &softSerial;
}

unsigned char *ColorDetector::readRGB()
{
    unsigned char i = 0, sum = 0;
    if (myserial->available()) {
        Re_buf[counter] = (unsigned char)myserial->read();
        if (counter == 0 && Re_buf[0] != 0x5A)
            return; // 检查帧头
        counter++;
        if (counter == 8) //接收到数据
        {
            counter = 0; //重新赋值，准备下一帧数据的接收
            sign = 1;
        }
    }

    if (sign && Re_buf[2] == 0x45) {
        sign = 0;
        for (i = 0; i < 7; i++)
            sum += Re_buf[i];
        if (sum == Re_buf[i]) //检查帧头，帧尾
        {
            rgb[0] = Re_buf[4];
            rgb[1] = Re_buf[5];
            rgb[2] = Re_buf[6];
            return rgb;
            /*
            Serial.print("r:");
            Serial.print(rgb[0]);
            Serial.print(",g:");
            Serial.print(rgb[1]);
            Serial.print(",b:");
            Serial.println(rgb[2]);
            */
        }
        else
            return NULL;
    }
    else 
        return NULL;
}