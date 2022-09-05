#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11); // arduino-RX, arduino-TX


unsigned char Re_buf[11],counter=0;
unsigned char sign=0;
byte rgb[3]={0};
void setup() {
   Serial.begin(9600);
   mySerial.begin(9600);
   mySerial.listen();  
   delay(10); 
   mySerial.write(0xA5);
   mySerial.write(0x60);
   mySerial.write(0x05);
   
   /*改变LED亮度
   mySerial.write(0xA5);
   mySerial.write(0x60);
   mySerial.write(0x05);
   */
   /* 输出rgb
   mySerial.write(0xA5); 
   mySerial.write(0x01);
   mySerial.write(0xA6);
   */
   
   mySerial.write(0xA5); 
   mySerial.write(0x82);
   mySerial.write(0x27);
} 

void loop() {
  unsigned char i=0,sum=0;
  while (mySerial.available()) {   
    Re_buf[counter]=(unsigned char)mySerial.read();
    if(counter==0&&Re_buf[0]!=0x5A) return;      // 检查帧头         
    counter++;       
    if(counter==11)                //接收到数据
    {    
       counter=0;                 //重新赋值，准备下一帧数据的接收 
       sign=1;
    }      
  }
  
  if(sign && Re_buf[2] == 0x45) //返回RGB数值
  {   
     sign=0;
     for(i=0;i<7;i++)
      sum+=Re_buf[i]; 
     if(sum==Re_buf[i] )        //检查帧头，帧尾
     {  	       
          rgb[0]=Re_buf[4];
          rgb[1]=Re_buf[5];
          rgb[2]=Re_buf[6];
           Serial.print("r:");
           Serial.print( rgb[0]);
           Serial.print(",g:");
           Serial.print( rgb[1]);
           Serial.print(",b:");
           Serial.println( rgb[2]); 
   }
  } 
  else if(sign && Re_buf[2] == 0x25) //返回颜色
  {   
     sign=0;
     for(i=0;i<11;i++) sum+=Re_buf[i]; 
     unsigned char color = (Re_buf[8]<<8)|Re_buf[9];
     switch(color){
      case 1: Serial.println("1红色");break;
      case 2: Serial.println("2黄色");break;
      case 4: Serial.println("3粉色");break;
      case 8: Serial.println("4白色");break;
      case 16: Serial.println("5黑色");break;
      case 32: Serial.println("6绿色");break;
      case 64: Serial.println("7深蓝色");break;
      case 128: Serial.println("8蓝色");break;
     }
   } 
}

int bin(int bi, int len){
  int i = 0;
  while(i<len){
    int tmp = 1;
    if((bi & (tmp<<(len-i-1)))!=0){
      Serial.print("1");
    }
    else{
      Serial.print("0");
    }
    i++;
  }
  Serial.print("\n");
  return 0;
}
