#include <Servo.h>

#define PIN 1

Servo myServo;

void setup() {
  pinMode(PIN, OUTPUT);
  myServo.attach(1);
}

void loop() {
  myServo.write(165);
  // tone(PIN, 1000);
  // noTone(PIN);
  delay(1000);
  myServo.write(180);
  delay(1000);
}
