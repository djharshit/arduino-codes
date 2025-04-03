const int buzzerPin = 5;

void setup() {
    pinMode(buzzerPin, OUTPUT);
}

void loop() {
    tone(buzzerPin, 1000); // Generate a 1kHz tone
    delay(1000);           // Beep for 1 second
    noTone(buzzerPin);     // Stop the tone
    delay(1000);           // Pause for 1 second
}
