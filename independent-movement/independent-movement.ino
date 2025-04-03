#include <ESP32Servo.h>

const int ledPin1 = 5;
const int ledPin2 = 16;
const int buzzerPin = 17;
const int servoPin = 18;
const int trigPin = ;
const int echoPin = ;
const int irPin = 13;

Servo servo;

// Timing variables
unsigned long previousLed1Time = 0;
unsigned long previousLed2Time = 0;
unsigned long previousBuzzerTime = 0;
unsigned long previousServoTime = 0;
unsigned long previousUltraTime = 0;
unsigned long previousIrTime = 0;

// Interval times (in milliseconds)
const int led1Interval = 500;
const int led2Interval = 500;
const int buzzerInterval = 1000;
const int servoInterval = 50;
const int ultraInterval = 1000;

// State variables
bool led1State = false;
bool led2State = true;
bool buzzerState = false;
bool servoState = true;
bool ultraState = true;

int servoAngle = 0;
int servoStep = 1;

void setup() {
  Serial.begin(9600);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servo.attach(servoPin);
}

void loop() {
  unsigned long currentMillis = millis();

  // Control LED 1
  if (currentMillis - previousLed1Time >= led1Interval) {
    previousLed1Time = currentMillis;
    led1State = !led1State;
    digitalWrite(ledPin1, led1State);
  }

  // Control LED 2
  if (currentMillis - previousLed2Time >= led2Interval) {
    previousLed2Time = currentMillis;
    led2State = !led2State;
    digitalWrite(ledPin2, led2State);
  }

  // Control Buzzer
  if (currentMillis - previousBuzzerTime >= buzzerInterval) {
    previousBuzzerTime = currentMillis;
    buzzerState = !buzzerState;
    // if (buzzerState) {
    //   tone(buzzerPin, 1000);
    // } else {
    //   noTone(buzzerPin);
    // }
  }

  // Servo Sweep Logic
  if (currentMillis - previousServoTime >= servoInterval) {
    previousServoTime = currentMillis;
    servoState = !servoState;

    // if (servoState) {
    //   servo.write(0);
    //   Serial.println(servoState);
    // } else {
    //   servo.write(180);
    //   Serial.println(servoState);
    // }
    
    servo.write(servoAngle);
    servoAngle += servoStep;
    // Serial.println(servoAngle);

    // Reverse direction at limits
    if (servoAngle >= 180 || servoAngle <= 0) {
      servoStep = -servoStep;
    }
  }
  
  // Ultrasonic Sensor Read
  if (currentMillis - previousUltraTime >= ultraInterval) {
    previousUltraTime = currentMillis;

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);  // Measure time for echo
    float distance = duration * 0.034 / 2;   // Convert to cm

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  }
}
