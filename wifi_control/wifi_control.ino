#include <vehicle.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>
#include <ultrasonic.h>
#include "LittleFS.h"

#define BUZZER 12
#define SERVO_1 25
#define SERVO_2 26
#define SERVO_3 27
#define SERVO_4 50
#define TRIGGER 14
#define ECHO 13
#define IR 2
#define LEFT_IR 35
#define MIDDLE_IR 36
#define RIGHT_IR 39

const char* ssid = "Redmi Note 12 Pro";
const char* password = "4N82TqGtaC";
int speeds = 255;
int chasis[3] = { 0, 100, 180 };
int shoulder[3] = { 10, 55, 100 };
int elbow[3] = { 120, 150, 180 };
int distance = 0;
int threshold = 400;
int chasisA, shoulderA, elbowA;
int lVal, mVal, rVal;
char buffer[50];
bool clawState = true;

vehicle myVeh;
Servo myServo1;
Servo myServo2;
Servo myServo3;
Servo myServo4;
ultrasonic myUltra;
WebServer server(80);

void setup() {
  Serial.begin(9600);
  pinMode(BUZZER, OUTPUT);
  pinMode(SERVO_1, OUTPUT);
  pinMode(SERVO_2, OUTPUT);
  pinMode(SERVO_3, OUTPUT);
  pinMode(SERVO_4, OUTPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(IR, INPUT);

  myUltra.Init(TRIGGER, ECHO);
  myServo1.attach(SERVO_1);
  myServo2.attach(SERVO_2);
  myServo3.attach(SERVO_3);
  myServo4.attach(SERVO_4);
  myVeh.Init();
  myVeh.Move(Stop, 0);

  chasisA = chasis[1];
  shoulderA = shoulder[1];
  elbowA = elbow[1];
  myServo1.write(chasisA);
  myServo2.write(shoulderA);
  myServo3.write(elbowA);

  if (!LittleFS.begin()) {
    Serial.println("Failed to mount LittleFS");
  } else {
    Serial.println("Mounting LittleFS successfully");
  }
  listFiles();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nConnected to Wi-Fi. ESP32 IP Address: ");
  Serial.println(WiFi.localIP());
  server.on("/", handleRoot);        //Enable the web page generation service
  server.on("/Car", handlemoveCar);  //Enable the car control service
  server.on("/Buzz", handleBuzzer);  // buzzer endpoint
  server.on("/Servo", handleServo);  // servo endpoint
  server.on("/Distance", handleDistance);
  server.begin();
  server.serveStatic("/", FSYSTEM, "/");
  Serial.println("Server started!");
}

void loop() {
  server.handleClient();  //Loop through user requests
}

void listFiles() {
  Serial.println("Listing files...");
  File root = FSYSTEM.open("/");
  File file = root.openNextFile();
  while (file) {
    Serial.println(file.name());
    file = root.openNextFile();
  }
}

void handleRoot() {
  const char message[] = R"rawliteral(
  <html>
  <head>
    <style>
      body {
        font-family: Arial, sans-serif;
        text-align: center;
        -webkit-user-select: none;
        user-select: none;
      }

      button {
        font-size: 20px;
        padding: 15px;
        margin: 10px;
        width: 150px;
        height: 80px;
        cursor: pointer;
      }

      p {
        font-size: 40px;
      }

      .control-btn {
        background-color: #007BFF;
        color: white;
        border: none;
      }

      #joystick-zone {
        width: 300px;
        height: 300px;
        margin: 30px auto;
        background-color: #f0f0f0;
        border-radius: 50%;
        position: relative;
      }
    </style>
    <script src="/nipplejs.min.js"></script>
  </head>

  <body>
    <h1>moveCar control</h1>
    <p><strong>Distance: </strong><span id="distance">--</span> cm</p>

    <div id="joystick-zone"></div>

    <center>
      <button onmousedown="toggleBuzz('on')" onmouseup="toggleBuzz('off')" ontouchstart="toggleBuzz('on')" ontouchend="toggleBuzz('off')" class='control-btn'>Beep</button>
      <button onmousedown="ledBlink('on')" onmouseup="ledBlink('off')" ontouchstart="ledBlink('on')" ontouchend="ledBlink('off')" class='control-btn'>LED</button>
    </center>

    <script>
      function moveCar(move) {
        fetch('/Car?move=' + move);
      }

      function toggleBuzz(state) {
        fetch('/Buzz?state=' + state);
      }

      function ledBlink(state) {
        fetch('/Led?state=' + state);
      }

      function updateDistance() {
        fetch('/Distance')
          .then(response => response.text())
          .then(data => {
            document.getElementById('distance').innerText = data;
          });
      }

      setInterval(updateDistance, 500);

      // 🕹 Joystick Setup
      var joystick = nipplejs.create({
        zone: document.getElementById('joystick-zone'),
        mode: 'static',
        position: { left: '50%', top: '50%' },
        color: 'blue'
      });

      let lastDirection = '';

      joystick.on('dir', function (evt, data) {
        if (!data || !data.direction) return;

        let direction = data.direction.angle;

        if (direction !== lastDirection) {
          lastDirection = direction;
          let moveCommand = 's';

          switch (direction) {
            case 'up':
              moveCommand = 'f';
              break;
            case 'down':
              moveCommand = 'b';
              break;
            case 'left':
              moveCommand = 'l';
              break;
            case 'right':
              moveCommand = 'r';
              break;
            case 'up-left':
              moveCommand = 'tl';
              break;
            case 'up-right':
              moveCommand = 'tr';
              break;
          }

          moveCar(moveCommand);
        }
      });

      joystick.on('end', function () {
        moveCar('s');
        lastDirection = '';
      });
    </script>
  </body>
  </html>
  )rawliteral";
  server.send(200, "text/html", message);
}

void handleBuzzer() {
  if (server.hasArg("state")) {
    String state = server.arg("state");
    Serial.print("Buzzer: ");
    Serial.println(state);
    if (state == "on") {
      tone(BUZZER, 1000);
    } else {
      noTone(BUZZER);
    }
  }
  server.send(200, "text/plain", "OK");
}

void handleDistance() {
  long distance = getDistance();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  server.send(200, "text/plain", String(distance));
}

long getDistance() {
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER, LOW);
  long duration = pulseIn(ECHO, HIGH);
  return duration * 0.034 / 2;
}

void handleServo() {
  if (server.hasArg("angle")) {
    int angle = server.arg("angle").toInt();
    Serial.print("Servo Angle: ");
    Serial.println(angle);
    myServo1.write(angle);  // Rotate servo to given angle
  }
  server.send(200, "text/plain", "OK");
}

void handlemoveCar() {  //Smart car control functions
  if (server.hasArg("move")) {
    String move = server.arg("move");
    Serial.println(move);
    if (move == "f") {
      myVeh.Move(Forward, speeds);
    } else if (move == "b") {
      myVeh.Move(Backward, speeds);
    } else if (move == "l") {
      myVeh.Move(Contrarotate, speeds);
    } else if (move == "r") {
      myVeh.Move(Clockwise, speeds);
    } else if (move == "s") {
      myVeh.Move(Stop, 0);
    } else if (move == "tl") {
      myVeh.Move(Move_Left, speeds);
    } else if (move == "tr") {
      myVeh.Move(Move_Right, speeds);
    }
  }
  server.send(200, "text/plain", "OK");
}