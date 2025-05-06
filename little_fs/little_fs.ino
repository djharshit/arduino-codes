#include "LittleFS.h"

void setup() {
  Serial.begin(9600);

  if (!LittleFS.begin()) {
    Serial.println("fail to load fs");
    return;
  } else {
    Serial.println("pass to load fs");
  }

  File file = LittleFS.open("/one.txt", "r");
  if (!file) {
    Serial.println("fail to load file");
    return;
  }
  while (file.available()){
    Serial.write(file.read());
  }
  file.close();
}

void loop() {
  // put your main code here, to run repeatedly:
}
