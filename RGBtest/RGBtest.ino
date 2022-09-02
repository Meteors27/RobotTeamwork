int red_light_pin= 5;
int green_light_pin = 7;
int blue_light_pin = 6;
void setup() {
  Serial.begin(9600);
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);
}
void loop() {
  Serial.println("red");
  RGB_color(255, 0, 0); // Red
  delay(1000);
  Serial.println("green");
  RGB_color(0, 255, 0); // Green
  delay(1000);
  Serial.println("blue");
  RGB_color(0, 0, 255); // Blue
  delay(1000);
  Serial.println("raspberry");
  RGB_color(255, 255, 125); // Raspberry
  delay(1000);
  Serial.println("cyan");
  RGB_color(0, 255, 255); // Cyan
  delay(1000);
  Serial.println("magenta");
  RGB_color(255, 0, 255); // Magenta
  delay(1000);
  Serial.println("yellow");
  RGB_color(255, 255, 0); // Yellow
  delay(1000);
  Serial.println("white");
  RGB_color(255, 255, 255); // White
  delay(1000);
  Serial.println("yadda");
  RGB_color(255, 255, 128);
  delay(1000);
  RGB_color(0,0,0);
  delay(1000);
}
void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
 {
  analogWrite(red_light_pin, 255 - red_light_value);
  analogWrite(green_light_pin, 255 - green_light_value);
  analogWrite(blue_light_pin, 255 - blue_light_value);
}
