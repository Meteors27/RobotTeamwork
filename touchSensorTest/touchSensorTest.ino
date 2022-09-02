int val;

void setup(){
    pinMode(8, INPUT);
    Serial.begin(9600);
}
//按下是0
void loop(){
    val = digitalRead(8);
    Serial.println(val);
    delay(200);
}
