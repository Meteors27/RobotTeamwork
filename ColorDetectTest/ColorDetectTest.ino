#include <SoftwareSerial.h>
#include <ColorDetector.h>

ColorDetector cd;

void setup() {
    Serial.begin(9600);
    cd.init();
}

void loop() {
    Serial.println(cd.color());
    delay(100);
}
    /*
    mySerial.write(0xA5);
    mySerial.write(0x60);
    mySerial.write(0x05);
    */
    /*改变LED亮度
    mySerial.write(0xA5);
    mySerial.write(0x60);
    mySerial.write(0x05);
    */
   /*
    if (MAX == 8) { // 输出rgb
        mySerial.write(0xA5);
        mySerial.write(0x81);
        mySerial.write(0x26);
    }
    else if (MAX == 11) { //输出颜色
        mySerial.write(0xA5);
        mySerial.write(0x82);
        mySerial.write(0x27);
    }
    */