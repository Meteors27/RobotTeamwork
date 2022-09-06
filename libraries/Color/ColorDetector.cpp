#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "ColorDetector.h"

#define judgement(a,b,c) (rgb[a] - rgb[b] > 25 && rgb[a] - rgb[c] > 25)
#define judgement2(a,b,c) (rgb[a] > 190 && rgb[b] < 190 && rgb[c] < 190)

/**
 * @brief 生成一个ColorDetector实例
 * @param RX 传感器DR引脚
 * @param TX 传感器CT引脚
 */
SoftwareSerial softserial(10, 11);

ColorDetector::ColorDetector()
{
    // softserial.SoftwareSerial(RX, TX);
    // SoftwareSerial cd(RX, TX);
    // softserial = &cd;
    counter = 0;
    sign = 0;
}

/**
 * @brief 颜色传感器初始化，调整亮度，配置参数
 * @return unsigned char* 
 */
void ColorDetector::init()
{
    softserial.begin(9600);
    /* 修改亮度 */
    softserial.listen();
    delay(10);
    softserial.write(0xA5); 
    softserial.write(0x60); 
    softserial.write(0x05); 

    /* 修改配置 */
    softserial.listen();
    delay(10);
    softserial.write(0xA5); 
    softserial.write((unsigned char)0x00);
    softserial.write(0xA5);
}

/**
 * @brief 读取RGB数值
 * @return unsigned char* 返回指向长度共3bytes的RGB数组指针
 */
unsigned char *ColorDetector::readRGB()
{
    unsigned char i = 0, sum = 0;
    softserial.listen();
    delay(10);
    softserial.write(0xA5);
    softserial.write(0x54);
    softserial.write(0xF9);
    //delay(10);
    if (softserial.available()) {
        Re_buf[counter] = (unsigned char)softserial.read();
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
    }
    return NULL;
}

String ColorDetector::color()
{
    readRGB();
    if(rgb == NULL) return "error";
    else if(judgement(0,1,2) || judgement2(0,1,2))
        return "red";
    else if(judgement(1,0,2) || (judgement2(1,0,2) && rgb[1]>200))
        return "green";
    else if(rgb[2]>=rgb[1] && rgb[2] > rgb[0] + 30|| judgement2(2,0,1))
        return "blue";
    else 
        return "unknown";
}
