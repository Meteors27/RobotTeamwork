#include <Servo.h>  
#include <CK008.h>                  
Servo servo_lowerArm, servo_middleArm, servo_upperArm, servo_hand;
Servo servo_roboticArm, servo_storageBox;
int cnt = 1;

#define CK008_PIN 43

CK008 ck008(CK008_PIN);

void block_placing(){
    rotate_roboticArm(177);
    delay(200);

    angle_hand(50);  //手张开
    delay(200);

    angle_lowerArm(135); //最底下的舵机到位
    delay(300);
    angle_middleArm(53);//第二个舵机到位
    delay(300);
    angle_upperArm(20); //最上面的舵机到位，打下来
    delay(300);

    angle_hand(95); //爪子合拢抓取
    delay(1000);

    angle_lowerArm_slow(110);//抬升
    delay(1000);

    rotate_roboticArm(45); //转180度
    delay(1000);

    angle_lowerArm(135); //下降，最底下的舵机到位
    delay(300);

    angle_hand(50); //爪子松开，放置
    delay(200);

    angle_lowerArm_slow(110);//抬升
    delay(1000);

    //下面是抓取
    angle_lowerArm(135); //最底下的舵机到位
    delay(300);

    angle_hand(95); //爪子合拢抓取
    delay(1000);

    angle_lowerArm_slow(110);//抬升
    delay(1000);
}

void setup(){
    while (1){
        if (ck008.detect() == TOUCHED)
            break;
    }

    servo_lowerArm.attach(17);
    servo_middleArm.attach(16);
    servo_upperArm.attach(15);
    servo_hand.attach(14);

    servo_roboticArm.attach(18);
    servo_storageBox.attach(20);

}

void loop(){
    block_placing();
}


void angle_hand(int angle){
    int now_angle;
    now_angle = servo_hand.read();

    if (angle < now_angle){
        for (int i = now_angle;i > angle;i--){
            servo_hand.write(i);
            delay(15);
        }
    }
    else if (angle > now_angle){
        for (int i = now_angle;i < angle;i++){
            servo_hand.write(i);
            delay(15);
        }
    }
}


void angle_upperArm(int angle){
    int now_angle;
    now_angle = servo_upperArm.read();

    if (angle < now_angle){
        for (int i = now_angle;i > angle;i--){
            servo_upperArm.write(i);
            delay(15);
        }
    }
    else if (angle > now_angle){
        for (int i = now_angle;i < angle;i++){
            servo_upperArm.write(i);
            delay(15);
        }
    }
}


void angle_middleArm(int angle){
    int now_angle;
    now_angle = servo_middleArm.read();

    if (angle < now_angle){
        for (int i = now_angle;i > angle;i--){
            servo_middleArm.write(i);
            delay(15);
        }
    }
    else if (angle > now_angle){
        for (int i = now_angle;i < angle;i++){
            servo_middleArm.write(i);
            delay(15);
        }
    }
}

void angle_lowerArm(int angle){
    int now_angle;
    now_angle = servo_lowerArm.read();

    if (angle < now_angle){
        for (int i = now_angle;i > angle;i--){
            servo_lowerArm.write(i);
            delay(15);
        }
    }
    else if (angle > now_angle){
        for (int i = now_angle;i < angle;i++){
            servo_lowerArm.write(i);
            delay(15);
        }
    }
}

void rotate_roboticArm(int angle){
    int now_angle;
    now_angle = servo_roboticArm.read();

    if (angle < now_angle){
        for (int i = now_angle;i > angle;i--){
            servo_roboticArm.write(i);
            delay(15);
        }
    }
    else if (angle > now_angle){
        for (int i = now_angle;i < angle;i++){
            servo_roboticArm.write(i);
            delay(15);
        }
    }
}

void rotate_storageBox(int angle){
    int now_angle;
    now_angle = servo_storageBox.read();

    if (angle < now_angle){
        for (int i = now_angle;i > angle;i--){
            servo_storageBox.write(i);
            delay(15);
        }
    }
    else if (angle > now_angle){
        for (int i = now_angle;i < angle;i++){
            servo_storageBox.write(i);
            delay(15);
        }
    }
}

void angle_lowerArm_slow(int angle){
    int now_angle;
    now_angle = servo_lowerArm.read();

    if (angle < now_angle){
        for (int i = now_angle;i > angle;i--){
            servo_lowerArm.write(i);
            delay(30);
        }
    }
    else if (angle > now_angle){
        for (int i = now_angle;i < angle;i++){
            servo_lowerArm.write(i);
            delay(30);
        }
    }
}
