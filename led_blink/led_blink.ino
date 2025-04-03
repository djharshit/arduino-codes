#define ledPin1 2
#define ledPin2 12

void setup() {
    pinMode(ledPin1, OUTPUT);
    pinMode(ledPin2, OUTPUT);
}

void loop() {
    digitalWrite(ledPin1, HIGH); // Turn LED on
    digitalWrite(ledPin2, LOW);
    delay(1000);                // Wait 1 second
    digitalWrite(ledPin1, LOW);  // Turn LED off
    digitalWrite(ledPin2, HIGH);
    delay(1000);                // Wait 1 second
}
