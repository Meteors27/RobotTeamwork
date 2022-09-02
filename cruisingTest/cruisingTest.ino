const int trackingPin = 8; 
const int a0 = A0;

void setup()
{
  pinMode(8, INPUT);
  Serial.begin(9600);
}

void loop()
{
  int val = analogRead(a0);
  Serial.println(val);
   
//  if(val < 200) 
//  { 
//    Serial.println("env");
//  }
//  else
//  {
//    //检测到了黑线1
//    Serial.println("line");
//  }
  delay(100);
}
