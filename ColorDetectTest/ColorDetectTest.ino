#include <ColorDetector.h>
#define S0 49
#define S1 51
#define S2 45
#define S3 43
#define OUT 47
ColorDetector cd(S0, S1, S2, S3, OUT);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("  r:");
  Serial.print(cd.red());
  Serial.print("  g:");
  Serial.print(cd.green());
  Serial.print("  b:");
  Serial.print(cd.blue());
  Serial.println();
  Serial.println(cd.color());
  delay(500);
}