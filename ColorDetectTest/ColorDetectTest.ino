#include <ColorDetector.h>

ColorDetector cd(5,4,7,6,8);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(cd.color());
  delay(500);
}