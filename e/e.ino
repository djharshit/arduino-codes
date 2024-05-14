#include <Servo.h>

Servo ser0;
Servo ser1;
Servo ser2;
Servo ser3;

int aP=A0, bP=A1, cP=A2, dP=A3;
int aV, bV, cV, dV, val;
char buffer[50];

void setup() {
  ser0.attach(5);
  ser1.attach(6);
  ser2.attach(9);
  ser3.attach(10);
  Serial.begin(9600);
}

void loop() {
  aV = analogRead(aP);
  bV = analogRead(bP);
  cV = analogRead(cP);
  dV = analogRead(dP);

  aV = map(aV, 0, 1023, 0, 180); 
  bV = map(bV, 0, 1023, 20, 60);
  cV = map(cV, 0, 1023, 120, 180);
  dV = map(dV, 0, 1023, 125, 150);

  ser0.write(aV);
  ser1.write(bV);
  ser2.write(cV);
  ser3.write(dV);

  sprintf(buffer, "%d %d %d %d", aV, bV, cV, dV);
  Serial.println(buffer);

  delay(200);
}
