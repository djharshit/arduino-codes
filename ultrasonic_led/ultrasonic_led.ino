const int ledPin = 9;  // Connect the passive buzzer to pin 9
const int trigPin = 11;   // Trigger pin of ultrasonic sensor
const int echoPin = 10;  // Echo pin of ultrasonic sensor

void setup() {
    Serial.begin(9600); // Start serial communication
    pinMode(ledPin, OUTPUT);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

void loop() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    long duration = pulseIn(echoPin, HIGH); // Measure time for echo
    float distance = duration * 0.034 / 2; // Convert to distance (cm)

    if (distance < 10) {
      digitalWrite(ledPin, HIGH); // Turn LED on
    }
    else {
      digitalWrite(ledPin, LOW);  // Turn LED off
    }
    
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    
    delay(500); // Delay before next reading
}
