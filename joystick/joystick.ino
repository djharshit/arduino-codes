#include <Servo.h>

Servo myservo;

int xPin = A0;
int yPin = A1;
int xValue, yValue, val;
char buffer[50];

void setup() {
  Serial.begin(9600);
  myservo.attach(9);
}

void loop() {
  // read the value from the sensor:
  xValue = analogRead(xPin);
  yValue = analogRead(yPin);

  val = map(xValue, 0, 1023, 0, 180);
  myservo.write(val);

  sprintf(buffer, "x %d y %d", xValue, yValue);  // Format and store the combined string in buffer
  Serial.println(buffer);
  delay(15);
}
