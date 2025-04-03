const int trigPin = 13;   // Trigger pin of ultrasonic sensor
const int echoPin = 14;  // Echo pin of ultrasonic sensor

void setup() {
    Serial.begin(9600); // Start serial communication
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
      
    }
    
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    
    delay(500); // Delay before next reading
}
