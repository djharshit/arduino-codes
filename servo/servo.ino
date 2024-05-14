#include <Servo.h>

Servo myservo;

int val, x;

void setup() {
  myservo.attach(4);
}

void loop() {
  myservo.write(120);
  // val = analogRead(3);
  // x = map(val, 0, 1023, 0, 180);
  // myservo.write(x);
  // delay(100);
}
