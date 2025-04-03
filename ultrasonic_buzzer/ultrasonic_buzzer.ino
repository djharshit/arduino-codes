const int buzzerPin = 9;  // Connect the passive buzzer to pin 9
const int trigPin = 11;   // Trigger pin of ultrasonic sensor
const int echoPin = 10;  // Echo pin of ultrasonic sensor

void setup() {
    Serial.begin(9600); // Start serial communication
    pinMode(buzzerPin, OUTPUT);
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
        tone(buzzerPin, 1000); // Generate a 1kHz tone
        delay(100);             // Beep for 1 second
        noTone(buzzerPin);     // Stop the tone
        delay(50);             // Pause for 1 second
    }
    
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    
    delay(500); // Delay before next reading
}
