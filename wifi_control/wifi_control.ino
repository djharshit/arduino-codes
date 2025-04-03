#include <vehicle.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

#define BUZZER_PIN 33
#define SERVO_PIN 27
#define LED1_PIN 2
#define LED2_PIN 12
#define TRIG_PIN 13
#define ECHO_PIN 14

const char* ssid = "Redmi Note 12 Pro";
const char* password = "4N82TqGtaC";
int Speeds = 255;

vehicle myVeh;
Servo myServo;
WebServer server(80);

void handleRoot() {
  const char message[] = R"rawliteral(
  <html>
  <style> 
  body { font-family: Arial, sans-serif; text-align: center; -webkit-user-select: none; -khtml-user-select: none;-moz-user-select: none;-ms-user-select: none;user-select: none;}
  button { font-size: 20px; padding: 15px; margin: 10px; width: 150px; height: 80px; cursor: pointer; }
  p { font-size: 40px;}
  .control-btn { background-color: #007BFF; color: white; border: none; }
  </style>
  <body>
  <center><h1>moveCar control</h1></center>
  <p><strong>Distance: </strong><span id="distance">--</span> cm</p>
  <center>
  <button onmousedown="moveCar('tl')" onmouseup="moveCar('s')" ontouchstart="moveCar('tl')" ontouchend="moveCar('s')" class='control-btn'>Turn left</button>
  <button onmousedown="moveCar('f')" onmouseup="moveCar('s')" ontouchstart="moveCar('f')" ontouchend="moveCar('s')" class='control-btn'>Forward</button>
  <button onmousedown="moveCar('tr')" onmouseup="moveCar('s')" ontouchstart="moveCar('tr')" ontouchend="moveCar('s')" class='control-btn'>Turn Right</button>
  </center>
  <center>
  <button onmousedown="moveCar('l')" onmouseup="moveCar('s')" ontouchstart="moveCar('l')" ontouchend="moveCar('s')" class='control-btn'>Left</button>
  <button onmousedown="moveCar('b')" onmouseup="moveCar('s')" ontouchstart="moveCar('b')" ontouchend="moveCar('s')" class='control-btn'>Backward</button>
  <button onmousedown="moveCar('r')" onmouseup="moveCar('s')" ontouchstart="moveCar('r')" ontouchend="moveCar('s')" class='control-btn'>Right</button>
  </center>
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
      .then(data => { document.getElementById('distance').innerText = data; });
  }
  setInterval(updateDistance, 500);
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
      tone(BUZZER_PIN, 1000);
    } 
    else {
      noTone(BUZZER_PIN);
    }
  }
  server.send(200, "text/plain", "OK");
}

void handleLed() {
  if (server.hasArg("state")) {
    String state = server.arg("state");
    Serial.print("LED: ");
    Serial.println(state);
    if (state == "on") {
      digitalWrite(LED1_PIN, HIGH);
      digitalWrite(LED2_PIN, HIGH);
    } 
    else {
      digitalWrite(LED1_PIN, LOW);
      digitalWrite(LED2_PIN, LOW);
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
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2;
}

void handleServo() {
  if (server.hasArg("angle")) {
    int angle = server.arg("angle").toInt();
    Serial.print("Servo Angle: ");
    Serial.println(angle);
    myServo.write(angle);  // Rotate servo to given angle
  }
  server.send(200, "text/plain", "OK");
}

void handlemoveCar() {  //Smart car control functions
  if (server.hasArg("move")) {
    String move = server.arg("move");
    Serial.println(move);
    if (move == "f") {
      myVeh.Move(Forward, Speeds);
    } else if (move == "b") {
      myVeh.Move(Backward, Speeds);
    } else if (move == "l") {
      myVeh.Move(Contrarotate, Speeds);
    } else if (move == "r") {
      myVeh.Move(Clockwise, Speeds);
    } else if (move == "s") {
      myVeh.Move(Stop, 0);
    } else if (move == "tl") {
      myVeh.Move(Move_Left, Speeds);
    } else if (move == "tr") {
      myVeh.Move(Move_Right, Speeds);
    }
  }
  server.send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  myServo.attach(SERVO_PIN);
  myServo.write(90); // Default servo position

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to Wi-Fi");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("\r\n");
  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());  //Print ESP32 IP infomation
  Serial.println("' to connect");
  server.on("/", handleRoot);        //Enable the web page generation service
  server.on("/Car", handlemoveCar);  //Enable the car control service
  server.on("/Buzz", handleBuzzer); // buzzer endpoint
  server.on("/Servo", handleServo); // servo endpoint
  server.on("/Led", handleLed);
  server.on("/Distance", handleDistance);
  server.begin();
  Serial.println("Server started!");
  myVeh.Init();
  myVeh.Move(Stop, 0);
}
void loop() {
  server.handleClient();  //Loop through user requests
}