#include <ultrasonic.h>

#define TrigPin 14   // Trigger pin of ultrasonic sensor
#define EchoPin 13  // Echo pin of ultrasonic sensor
int distance = 0;

ultrasonic myUltra;

void setup() {
    Serial.begin(9600);
    pinMode(TrigPin, OUTPUT);
    pinMode(EchoPin, INPUT);
    myUltra.Init(TrigPin, EchoPin);
}

void loop() {
    distance = myUltra.Ranging();

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    
    delay(500); // Delay before next reading
}
