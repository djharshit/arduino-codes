#include <vehicle.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>
#include <ultrasonic.h>
#include <AsyncTCP.h>
#include "LittleFS.h"
#include "FS.h"

#define BUZZER 12
#define SERVO_1 25
#define SERVO_2 26
#define SERVO_3 27
#define SERVO_4 4
#define TRIGGER 14
#define ECHO 13
#define LED 2
#define LEFT_IR 35
#define MIDDLE_IR 36
#define RIGHT_IR 39

#define NOTE_E 659
#define NOTE_C_SHARP 554
#define NOTE_B 493
#define NOTE_A 440
#define NOTE_G_SHARP 415
#define NOTE_F_SHARP 370
#define NOTE_D_SHARP 622
#define NOTE_A_SHARP 466

const char* ssid = "Major Project";
const char* password = "12345678";
unsigned long m1 = 0, m2 = 0, m3 = 0;
int lVal, mVal, rVal;
int beepCount = 0, speed = 255, chassisA = 100, shoulderA = 55, elbowA = 150, currentA = 0, note = 0, musicNo = 0, threshold = 2000, offRoad = 4000;
bool isBeeping = false, avoidance = false, following = false, tracking = false, buzzerOn = false, ledOn = false, clawState = true;
long distance = 0;
char buffer[50];
String move, arm;

int pardesi[][2] = {
  { NOTE_A_SHARP, 500 }, { NOTE_G_SHARP, 250 }, { NOTE_F_SHARP, 500 }, { NOTE_A_SHARP, 250 }, { NOTE_G_SHARP, 500 }, { NOTE_F_SHARP, 250 }, { NOTE_F_SHARP, 500 }, { NOTE_G_SHARP, 250 }, { NOTE_F_SHARP, 500 }, { NOTE_D_SHARP, 500 }, { NOTE_A_SHARP, 500 }, { NOTE_G_SHARP, 250 }, { NOTE_F_SHARP, 500 }, { NOTE_A_SHARP, 250 }, { NOTE_G_SHARP, 500 }, { NOTE_F_SHARP, 250 }, { NOTE_F_SHARP, 500 }, { NOTE_G_SHARP, 250 }, { NOTE_F_SHARP, 500 }, { NOTE_D_SHARP, 500 }, { NOTE_D_SHARP, 500 }, { NOTE_F_SHARP, 250 }, { NOTE_A_SHARP, 250 }, { NOTE_G_SHARP, 250 }, { NOTE_G_SHARP, 250 }, { NOTE_A_SHARP, 250 }, { NOTE_F_SHARP, 250 }, { NOTE_G_SHARP, 250 }, { NOTE_A_SHARP, 250 }, { NOTE_A_SHARP, 500 }, { NOTE_A_SHARP, 250 }, { NOTE_G_SHARP, 500 }, { NOTE_F_SHARP, 250 }, { NOTE_F_SHARP, 500 }, { NOTE_C_SHARP, 500 }, { NOTE_A_SHARP, 250 }, { NOTE_C_SHARP, 500 }, { NOTE_C_SHARP, 500 }, { NOTE_C_SHARP, 250 }, { NOTE_D_SHARP, 500 }, { NOTE_C_SHARP, 500 }, { NOTE_D_SHARP, 250 }, { NOTE_C_SHARP, 500 }, { NOTE_A_SHARP, 500 }, { NOTE_A_SHARP, 250 }, { NOTE_A_SHARP, 500 }, { NOTE_C_SHARP, 500 }, { NOTE_C_SHARP, 500 }, { NOTE_C_SHARP, 500 }, { NOTE_D_SHARP, 500 }, { NOTE_C_SHARP, 500 }, { NOTE_D_SHARP, 250 }, { NOTE_E, 500 }, { NOTE_D_SHARP, 250 }, { NOTE_C_SHARP, 250 }, { NOTE_A_SHARP, 500 }, { NOTE_C_SHARP, 250 }, { NOTE_A_SHARP, 500 }, { NOTE_G_SHARP, 250 }, { NOTE_F_SHARP, 250 }
};

vehicle myVeh;
Servo myServo1;
Servo myServo2;
Servo myServo3;
Servo myServo4;
ultrasonic myUltra;
AsyncWebServer server(80);

void setup() {
  Serial.begin(9600);

  // LittleFS
  if (!LittleFS.begin()) {
    Serial.println(F("Fail to load Little FS."));
    return;
  } else {
    Serial.println(F("Little FS loaded successfuly."));
  }
  listFilesInLittleFS();

  // Initialisation
  pinMode(BUZZER, OUTPUT);
  pinMode(SERVO_1, OUTPUT);
  pinMode(SERVO_2, OUTPUT);
  pinMode(SERVO_3, OUTPUT);
  pinMode(SERVO_4, OUTPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LED, OUTPUT);

  myUltra.Init(TRIGGER, ECHO);
  myServo1.attach(SERVO_1);
  myServo2.attach(SERVO_2);
  myServo3.attach(SERVO_3);
  myServo4.attach(SERVO_4);
  myVeh.Init();
  myVeh.Move(Stop, 0);

  // Wifi
  WiFi.setTxPower(WIFI_POWER_19_5dBm);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  Serial.print(F("Access Point start. Use "));
  Serial.print(WiFi.softAPIP());
  Serial.print(F("to connect"));

  // Webserver
  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");
  server.on("/Arm", HTTP_GET, handleArm);
  server.on("/Buzz", HTTP_GET, handleBuzzer);
  server.on("/Car", HTTP_GET, handleMoveCar);
  server.on("/Claw", HTTP_GET, handleClaw);
  server.on("/Led", HTTP_GET, handleLed);
  server.on("/Reset", HTTP_GET, handleReset);
  server.on("/Avoidance", HTTP_GET, [](AsyncWebServerRequest* request) {
    avoidance = true;
    following = false;
    tracking = false;
    request->send(200, "text/plain", "Started avoidance");
  });
  server.on("/Distance", HTTP_GET, [](AsyncWebServerRequest* request) {
    distance = myUltra.Ranging();
    Serial.println(distance);
    request->send(200, "text/plain", String(distance));
  });
  server.on("/Follow", HTTP_GET, [](AsyncWebServerRequest* request) {
    following = true;
    avoidance = false;
    tracking = false;
    request->send(200, "text/plain", "Started following");
  });
  server.on("/Music", HTTP_GET, [](AsyncWebServerRequest* request) {
    musicNo = request->getParam("track")->value().toInt();
    request->send(200, "text/plain", "Music start");
  });
  server.on("/Track", HTTP_GET, [](AsyncWebServerRequest* request) {
    tracking = true;
    following = false;
    avoidance = false;
    request->send(200, "text/plain", "Started tracking");
  });

  server.begin();

  // Servo Init
  myServo1.write(chassisA);
  myServo2.write(shoulderA);
  myServo3.write(elbowA);
}

void loop() {
  // Music playing
  if (musicNo == 1) {
    Serial.println(F("Music 1 playing"));
    if (note < 60) {
      if (millis() - m3 >= pardesi[note][1]) {
        tone(BUZZER, pardesi[note][0]);
        m3 = millis();
        note++;
      }
    } else {
      noTone(BUZZER);
      note = 0;
    }
  }

  // Buzzer beep
  if (isBeeping) {
    if (millis() - m1 >= 500) {
      m1 = millis();
      if (buzzerOn) {
        noTone(BUZZER);
        buzzerOn = false;
        beepCount++;
      } else if (beepCount < 3) {
        tone(BUZZER, 1000);
        buzzerOn = true;
        Serial.println(F("Buzzer beeping"));
      } else {
        isBeeping = false;
        noTone(BUZZER);
      }
    }
  }

  if (millis() - m2 >= 100) {
    m2 = millis();
    distance = myUltra.Ranging();

    // Object Avoidance
    if (avoidance) {
      Serial.println(F("Object Avoidance"));
      if (distance > 20) {
        myVeh.Move(Forward, speed);
        Serial.println(F("Moving..."));
      } else {
        myVeh.Move(Stop, 0);
        avoidance = false;
        Serial.println(F("Stopped"));
      }
    }

    // Object Following
    if (following) {
      Serial.println(F("Object Following"));
      if (distance < 20 && distance > 10) {
        myVeh.Move(Forward, speed);
        Serial.println(F("Moving..."));
      } else {
        myVeh.Move(Stop, 0);
        Serial.println(F("Stopped"));
      }
    }

    // Line Tracking
    if (tracking) {
      Serial.println(F("Line Tracking"));
      lVal = analogRead(LEFT_IR);
      mVal = analogRead(MIDDLE_IR);
      rVal = analogRead(RIGHT_IR);
      sprintf(buffer, "l %d m %d r %d", lVal, mVal, rVal);
      Serial.println(buffer);

      if (lVal < threshold && mVal >= threshold && rVal < threshold) {
        myVeh.Move(Forward, 180);
      }
      if (lVal < threshold && mVal >= threshold && rVal >= threshold) {
        myVeh.Move(Forward, 180);
      }
      if (lVal >= threshold && mVal >= threshold && rVal < threshold) {
        myVeh.Move(Forward, 180);
      } else if (lVal >= threshold && mVal < threshold && rVal < threshold) {
        myVeh.Move(Contrarotate, 220);
      } else if (lVal < threshold && mVal < threshold && rVal >= threshold) {
        myVeh.Move(Clockwise, 220);
      } else if (lVal >= offRoad && mVal >= offRoad && rVal >= offRoad) {
        myVeh.Move(Stop, 0);
      }
    }
  }
}

void handleMoveCar(AsyncWebServerRequest* request) {
  move = request->getParam("move")->value();
  Serial.println(move);
  if (move == "f") {
    myVeh.Move(Forward, speed);
  } else if (move == "b") {
    myVeh.Move(Backward, speed);
  } else if (move == "l") {
    myVeh.Move(Contrarotate, speed);
  } else if (move == "r") {
    myVeh.Move(Clockwise, speed);
  } else if (move == "s") {
    myVeh.Move(Stop, 0);
  } else if (move == "tl") {
    myVeh.Move(Move_Left, speed);
  } else if (move == "tr") {
    myVeh.Move(Move_Right, speed);
  }
  request->send(200, "text/plain", "Car moving: " + move);
}

void handleArm(AsyncWebServerRequest* request) {
  arm = request->getParam("arm")->value();
  currentA = request->getParam("angle")->value().toInt();
  Serial.print(arm);
  Serial.print(F(" "));
  Serial.println(currentA);
  if (arm == "chassis") {
    smoothMove(myServo1, chassisA, currentA);
    chassisA = currentA;
  } else if (arm == "shoulder") {
    smoothMove(myServo2, shoulderA, currentA);
    shoulderA = currentA;
  } else if (arm == "elbow") {
    smoothMove(myServo3, elbowA, currentA);
    elbowA = currentA;
  }
  request->send(200, "text/plain", "Arm moved");
}

void handleBuzzer(AsyncWebServerRequest* request) {
  if (!isBeeping) {
    beepCount = 0;
    isBeeping = true;
    m1 = millis();
  }
  request->send(200, "text/plain", "Buzzer start");
}

void handleLed(AsyncWebServerRequest* request) {
  if (ledOn) {
    digitalWrite(LED, 0);
    ledOn = false;
    Serial.println(F("Led Off"));
    request->send(200, "text/plain", "LED Off");
  } else {
    digitalWrite(LED, 1);
    ledOn = true;
    Serial.println(F("Led On"));
    request->send(200, "text/plain", "LED On");
  }
}
void handleClaw(AsyncWebServerRequest* request) {
  if (clawState) {
    myServo4.write(160);
    clawState = false;  // claw close
    Serial.println(F("Claw close"));
    request->send(200, "text/plain", "Claw close");
  } else {
    myServo4.write(180);
    clawState = true;  // claw open
    Serial.println(F("Claw open"));
    request->send(200, "text/plain", "Claw open");
  }
}

void handleReset(AsyncWebServerRequest* request) {
  smoothMove(myServo1, chassisA, 100);
  smoothMove(myServo2, shoulderA, 55);
  smoothMove(myServo3, elbowA, 150);
  chassisA = 100;
  shoulderA = 55;
  elbowA = 150;
  myVeh.Move(Stop, 0);
  noTone(BUZZER);
  isBeeping = false;
  following = false;
  avoidance = false;
  tracking = false;
  musicNo = 0;
  Serial.println(F("All reset"));
  request->send(200, "text/plain", "Reset");
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

void listFilesInLittleFS() {
  Serial.println(F("Listing files in LittleFS:"));
  File root = LittleFS.open("/");
  if (!root || !root.isDirectory()) {
    Serial.println(F("Failed to open LittleFS root or not a directory"));
    return;
  }
  File file = root.openNextFile();
  while (file) {
    Serial.print(file.name());
    Serial.print(F(" "));
    Serial.println(file.size());
    file = root.openNextFile();
  }
}