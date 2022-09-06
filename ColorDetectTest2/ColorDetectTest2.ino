/* 测试情况：rgb输出一般只会在几个值之间浮动。 */

#include <ColorDetector.h>

ColorDetector cd;

void setup() {
    Serial.begin(9600);
    Serial.println("hello world");
    
}

void loop() {
    unsigned char* rgb = cd.readRGB();
    if(rgb == NULL){
        Serial.print("R");
        Serial.print(rgb[0]);
        Serial.print("    G");
        Serial.print(rgb[1]);
        Serial.print("    B");
        Serial.println(rgb[2]);
    }
    else
        Serial.println("Error!");
}

/**
 * @brief Helper function for testing.
 * 
 * @param bi 
 * @param len 
 * @return int 
 */
int bin(int bi, int len) {
    int i = 0;
    while (i < len) {
        int tmp = 1;
        if ((bi & (tmp << (len - i - 1))) != 0) {
            Serial.print("1");
        }
        else {
            Serial.print("0");
        }
        i++;
    }
    Serial.print("\n");
    return 0;
}
