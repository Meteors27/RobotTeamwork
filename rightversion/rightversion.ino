#include <Servo.h>  
#include <CK008.h>                  
Servo servo_lowerArm,servo_middleArm,servo_upperArm,servo_hand; 
Servo servo_roboticArm,servo_storageBox;
int cnt=1;

#define CK008_PIN 43
#define STORAGEBOX_1 5 
#define STORAGEBOX_2 90
#define STORAGEBOX_3 180
#define speedrate 0.33

CK008 ck008(CK008_PIN);

typedef struct armmm
{
  int roboticArm;
  int lowerArm;
  int middleArm;
  int upperArm;
}armstatus;

armstatus back_up={5,85,53,20},back_down={5,110,53,20},forward_up={135,85,53,20},forward_down={135,115,53,20};
armstatus leftback_up={5,85,53,20},leftback_down={5,110,53,20},leftforward_up={155,110,32,10},leftforward_down={155,140,10,0};
armstatus rightback_up={5,85,53,20},rightback_down={5,110,53,20},rightforward_up={120,110,32,10},rightforward_down={120,140,10,0};

void rotate_to(int target_angle, Servo* servo){
    int now_angle;
    now_angle = servo->read();

    if (target_angle < now_angle){
        for (int i = now_angle;i > target_angle;i--){
            servo->write(i);
            delay(15);
        }
    }
    else if (target_angle > now_angle){
        for (int i = now_angle;i < target_angle;i++){
            servo->write(i);
            delay(15);
        }
    }
}

void rotate_arm(armstatus ARMSTATUS,int rotate_mode)
{
  switch(rotate_mode)
  {
    case 1:
      rotate_to(ARMSTATUS.middleArm,&servo_middleArm);
      delay(300*speedrate);
      rotate_to(ARMSTATUS.upperArm,&servo_upperArm);
      delay(300*speedrate); 
      rotate_to(ARMSTATUS.roboticArm,&servo_roboticArm);
      delay(100*speedrate);
      rotate_to(ARMSTATUS.lowerArm,&servo_lowerArm);
      delay(300*speedrate);
      break;
    case 2:
      rotate_to(ARMSTATUS.roboticArm,&servo_roboticArm);
      delay(100*speedrate);
      rotate_to(ARMSTATUS.lowerArm,&servo_lowerArm);
      delay(300*speedrate);
      rotate_to(ARMSTATUS.middleArm,&servo_middleArm);
      delay(300*speedrate);
      rotate_to(ARMSTATUS.upperArm,&servo_upperArm);
      delay(300*speedrate); 
      break;
     case 3:
      rotate_to(ARMSTATUS.lowerArm,&servo_lowerArm);
      delay(300*speedrate);
      rotate_to(ARMSTATUS.upperArm,&servo_upperArm);
      delay(300*speedrate); 
      rotate_to(ARMSTATUS.middleArm,&servo_middleArm);
      delay(300*speedrate);
      rotate_to(ARMSTATUS.roboticArm,&servo_roboticArm);
      delay(100*speedrate);
      break;
  }
  return;
}

void hand_open()
{
  rotate_to(60,&servo_hand);
  delay(300*speedrate);
  return;  
}

void hand_close()
{
  rotate_to(95,&servo_hand);
  delay(300*speedrate);
  return;  
}

void block_grabbing()
{
  hand_open();
  rotate_arm(back_up,1);
  
  rotate_arm(leftforward_up,3);
  rotate_arm(leftforward_down,1);
  hand_close();
  rotate_arm(leftforward_up,3);
  rotate_arm(back_up,2);
  rotate_arm(back_down,1);
  hand_open();
  rotate_arm(back_up,1);//grab left block

  rotate_arm(forward_up,2);
  hand_open();
  rotate_arm(forward_down,1);
  hand_close();
  rotate_arm(forward_up,1);
  rotate_arm(back_up,3);
  rotate_arm(back_down,1);
  hand_open();
  rotate_arm(back_up,1);//grab middle block
  
  rotate_arm(rightforward_up,3);
  rotate_arm(rightforward_down,1);
  hand_close();
  rotate_arm(rightforward_up,3);
  rotate_arm(back_up,2);
  rotate_arm(back_down,1);
  hand_open();
  rotate_arm(back_up,1);//grab right block
}

void block_placing()
{
  rotate_arm(back_up,1);

  rotate_arm(back_down,1);
  hand_close();
  rotate_arm(back_up,1);
  rotate_arm(rightforward_up,3);
  rotate_arm(rightforward_down,1);
  hand_open();
  rotate_arm(rightforward_up,3);
  rotate_arm(back_up,2);//place right block
  
  rotate_arm(back_down,1);
  hand_close();
  rotate_arm(back_up,1);
  rotate_arm(forward_up,2);
  rotate_arm(forward_down,1);
  hand_open();
  rotate_arm(forward_up,1);
  rotate_arm(back_up,3);//place middle block

  rotate_arm(back_down,1);
  hand_close();
  rotate_arm(back_up,1);+
  rotate_arm(leftforward_up,3);
  rotate_arm(leftforward_down,1);
  hand_open();
  rotate_arm(leftforward_up,3);
  rotate_arm(back_up,2);//place left block
}

void setup()                           
{ 
 servo_lowerArm.attach(4);
 servo_middleArm.attach(3); 
 servo_upperArm.attach(2);
 servo_hand.attach(1);

 servo_roboticArm.attach(5);
 servo_storageBox.attach(6);

}

void loop()                             
{
  
  if(cnt==1){
     block_grabbing();
     cnt=0;
  }
}
