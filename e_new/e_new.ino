#include <IRremote.hpp>

#define DECODE_DENON
#define DECODE_JVC
#define DECODE_KASEIKYO
#define DECODE_PANASONIC
#define DECODE_LG
#define DECODE_NEC
#define DECODE_SAMSUNG
#define DECODE_SONY
#define DECODE_RC5
#define DECODE_RC6
#define DECODE_BOSEWAVE
#define DECODE_LEGO_PF
#define DECODE_MAGIQUEST
#define DECODE_WHYNTER
#define DECODE_FAST
#define DECODE_DISTANCE_WIDTH
#define DECODE_HASH
#define DECODE_BEO
#define DEBUG
#define RAW_BUFFER_LENGTH 750

#include <IRremote.hpp>
#include <vehicle.h>
#include <Servo.h>
#include <ultrasonic.h>

#define SERVO_1 4
#define SERVO_2 3
#define SERVO_3 2
#define SERVO_4 50
#define IR_RECEIVE 7
#define ECHO_PIN 5
#define TRIGGER_PIN 6
#define LEFT_IR A3
#define MIDDLE_IR A4
#define RIGHT_IR A5
#define BUZZER 88

int speed = 255;
int chasis[3] = { 0, 100, 180 };
int shoulder[3] = { 10, 55, 100 };
int elbow[3] = { 120, 150, 180 };
int distance = 0;
int threshold = 400;
int chasisA, shoulderA, elbowA;
int lVal, mVal, rVal;
char buffer[50];
bool clawState = true;

Servo myServo1;
Servo myServo2;
Servo myServo3;
Servo myServo4;
vehicle myVeh;
ultrasonic myUltra;

void setup() {
  Serial.begin(9600);
  Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));
  IrReceiver.begin(IR_RECEIVE, ENABLE_LED_FEEDBACK);
  Serial.print(F("Ready to receive IR signals of protocols: "));
  printActiveIRProtocols(&Serial);
  Serial.print(F("at pin "));
  Serial.println(IR_RECEIVE);
  pinMode(LEFT_IR, INPUT);
  pinMode(MIDDLE_IR, INPUT);
  pinMode(RIGHT_IR, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(SERVO_1, OUTPUT);
  pinMode(SERVO_2, OUTPUT);
  pinMode(SERVO_3, OUTPUT);
  pinMode(SERVO_4, OUTPUT);
  myServo1.attach(SERVO_1);
  myServo2.attach(SERVO_2);
  myServo3.attach(SERVO_3);
  myServo4.attach(SERVO_4);
  myUltra.Init(TRIGGER_PIN, ECHO_PIN);
  myVeh.Init();
  myVeh.Move(Stop, 0);

  chasisA = chasis[1];
  shoulderA = shoulder[1];
  elbowA = elbow[1];
  myServo1.write(chasisA);
  myServo2.write(shoulderA);
  myServo3.write(elbowA);
}

void loop() {
  if (IrReceiver.decode()) {
    if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
      Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
      IrReceiver.printIRResultRawFormatted(&Serial, true);
      IrReceiver.resume();
    } else {
      IrReceiver.resume();
      IrReceiver.printIRResultShort(&Serial);
      IrReceiver.printIRSendUsage(&Serial);
    }
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
      Serial.println(F("Repeat received. Here you can repeat the same action as before."));
    } else {
      if (IrReceiver.decodedIRData.command == 0x4B) {
        myVeh.Move(Stop, 0);
      } else if (IrReceiver.decodedIRData.command == 0x5F) {
        myVeh.Move(Forward, speed);
      } else if (IrReceiver.decodedIRData.command == 0x54) {
        myVeh.Move(Backward, speed);
      } else if (IrReceiver.decodedIRData.command == 0x59) {
        myVeh.Move(Move_Left, speed);
      } else if (IrReceiver.decodedIRData.command == 0x58) {
        myVeh.Move(Move_Right, speed);
      } else if (IrReceiver.decodedIRData.command == 0x51) {
        myVeh.Move(Contrarotate, speed);
      } else if (IrReceiver.decodedIRData.command == 0x52) {
        myVeh.Move(Clockwise, speed);
      } else if (IrReceiver.decodedIRData.command == 0x1) {
        smoothMove(myServo1, chasisA, chasis[2]);
        chasisA = chasis[2];
      } else if (IrReceiver.decodedIRData.command == 0x2) {
        smoothMove(myServo1, chasisA, chasis[1]);
        chasisA = chasis[1];
      } else if (IrReceiver.decodedIRData.command == 0x3) {
        smoothMove(myServo1, chasisA, chasis[0]);
        chasisA = chasis[0];
      } else if (IrReceiver.decodedIRData.command == 0x4) {
        smoothMove(myServo2, shoulderA, shoulder[0]);
        shoulderA = shoulder[0];
      } else if (IrReceiver.decodedIRData.command == 0x5) {
        smoothMove(myServo2, shoulderA, shoulder[1]);
        shoulderA = shoulder[1];
      } else if (IrReceiver.decodedIRData.command == 0x6) {
        if (elbowA == elbow[2]) {
          smoothMove(myServo2, shoulderA, 60);
          shoulderA = 60;
        } else {
          smoothMove(myServo2, shoulderA, shoulder[2]);
          shoulderA = shoulder[2];
        }
      } else if (IrReceiver.decodedIRData.command == 0x7) {
        smoothMove(myServo3, elbowA, elbow[0]);
        elbowA = elbow[0];
      } else if (IrReceiver.decodedIRData.command == 0x8) {
        smoothMove(myServo3, elbowA, elbow[1]);
        elbowA = elbow[1];
      } else if (IrReceiver.decodedIRData.command == 0x9) {
        if (shoulderA == shoulder[2]) {
          smoothMove(myServo3, elbowA, 150);
          elbowA = 150;
        } else {
          smoothMove(myServo3, elbowA, elbow[2]);
          elbowA = elbow[2];
        }
      } else if (IrReceiver.decodedIRData.command == 0x0) {
        if (clawState) {
          myServo4.write(160);
          clawState = false;  // claw close
        } else {
          myServo4.write(180);
          clawState = true;  // claw open
        }
      } else if (IrReceiver.decodedIRData.command == 0x16) {
        // objectFollowing();
      } else if (IrReceiver.decodedIRData.command == 0xC) {
        // objectDetect();
      } else if (IrReceiver.decodedIRData.command == 0x43) {
        // lineTracing();
      } else if (IrReceiver.decodedIRData.command == 0xE) {
        // objectAvoidance();
      } else if (IrReceiver.decodedIRData.command == 0x10) {
        // buzzBeep();
      }
    }
  }
}

void objectDetect() {
  myVeh.Move(Forward, speed);
  while (true) {
    distance = myUltra.Ranging();
    // Serial.print(distance);
    // Serial.println(F(" cm"));
    if (distance < 15) {
      myVeh.Move(Stop, 0);
      // Serial.println(F("object detected"));
      break;
    }
    delay(100);
  }
}

void objectFollowing() {
  while (true) {
    distance = myUltra.Ranging();
    // Serial.print(distance);
    // Serial.println(F(" cm"));
    if (distance < 20 && distance > 10) {
      myVeh.Move(Forward, speed);
      // Serial.println(F("moving..."));
    } else {
      myVeh.Move(Stop, 0);
    }
    delay(100);
  }
}

void objectAvoidance() {
  myVeh.Move(Forward, speed);
  while (true) {
    distance = myUltra.Ranging();
    Serial.print(distance);
    Serial.println(F(" cm"));
    if (distance < 15) {
      myVeh.Move(Stop, 0);
      Serial.println(F("stopped....object detected"));
      myServo4.write(180);  // claw open
      delay(100);
      Serial.print(F("7 "));
      smoothMove(myServo3, elbowA, elbow[0]);  // 7
      elbowA = elbow[0];
      delay(100);
      Serial.print(F("6 "));
      smoothMove(myServo2, shoulderA, shoulder[2]);  // 6
      shoulderA = shoulder[2];
      delay(100);
      myServo4.write(160);                           // claw close
      Serial.print(F("5 "));
      smoothMove(myServo2, shoulderA, shoulder[1]);  // 5
      shoulderA = shoulder[1];
      delay(100);
      Serial.print(F("1 "));
      smoothMove(myServo1, chasisA, chasis[2]);  // 1
      chasisA = chasis[2];
      delay(100);
      myServo4.write(180);  // claw open
      delay(100);
      myServo4.write(160);  // claw close
      delay(100);
      Serial.print(F("2 "));
      smoothMove(myServo1, chasisA, chasis[1]);  // 2
      chasisA = chasis[1];
      delay(100);
      Serial.print(F("8 "));
      smoothMove(myServo3, elbowA, elbow[1]);  // 8
      elbowA = elbow[1];
      delay(100);
      Serial.println(F("moving..."));
      myVeh.Move(Forward, speed);
    }
    delay(100);
  }
}

void lineTracing() {
  while (true) {
    lVal = analogRead(LEFT_IR);
    mVal = analogRead(MIDDLE_IR);
    rVal = analogRead(RIGHT_IR);
    // sprintf(buffer, "l %d m %d r %d", lVal, mVal, rVal);
    // Serial.println(buffer);
    if (lVal < threshold && mVal >= threshold && rVal < threshold) {
      // Serial.println("forward 1...");
      myVeh.Move(Forward, 250);
    }
    if (lVal < threshold && mVal >= threshold && rVal >= threshold) {
      // Serial.println("forward 2...");
      myVeh.Move(Forward, 180);
    }
    if (lVal >= threshold && mVal >= threshold && rVal < threshold) {
      // Serial.println("forward 3...");
      myVeh.Move(Forward, 180);
    }
    if (lVal >= threshold && mVal < threshold && rVal < threshold) {
      // Serial.println("contrarotate...");
      myVeh.Move(Contrarotate, 220);
    }
    if (lVal < threshold && mVal < threshold && rVal >= threshold) {
      // Serial.println("clockwise...");
      myVeh.Move(Clockwise, 220);
    }
    if (lVal >= threshold && mVal >= threshold && rVal >= threshold) {
      // Serial.println("forward 4...");
      myVeh.Move(Forward, 180);
    }
    if (lVal <= threshold && mVal <= threshold && rVal <= threshold) {
      // Serial.println("stop...");
      myVeh.Move(Stop, 0);
      break;
    }
    delay(50);
  }
}

void buzzBeep() {
  tone(BUZZER, 1000);
  delay(2000);
  noTone(BUZZER);
}

void smoothMove(Servo& servo, int current, int target) {
  if (current == target) return;
  int step = (current < target) ? 5 : -5;
  for (int angle = current; (step > 0) ? (angle <= target) : (angle >= target); angle += step) {
    servo.write(angle);
    delay(25);
  }
  servo.write(target);
}