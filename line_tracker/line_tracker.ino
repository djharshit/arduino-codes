#define LPIN A3
#define MPIN A4
#define RPIN A5

int lVal, mVal, rVal;
char buffer[50];

void setup() {
  Serial.begin(9600);
  pinMode(LPIN, INPUT);
	pinMode(MPIN, INPUT);
	pinMode(RPIN, INPUT);
}

void loop() {
  lVal = analogRead(LPIN);
  mVal = analogRead(MPIN);
  rVal = analogRead(RPIN);

  sprintf(buffer, "l %d m %d r %d", lVal, mVal, rVal);
  Serial.println(buffer);
  delay(100);
}
