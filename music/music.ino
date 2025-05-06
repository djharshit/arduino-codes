// Pin connected to the buzzer
const int buzzerPin = 12;

#define NOTE_E 659
#define NOTE_C_SHARP 554
#define NOTE_B 493
#define NOTE_A 440
#define NOTE_G_SHARP 415
#define NOTE_F_SHARP 370
#define NOTE_D_SHARP 622
#define NOTE_A_SHARP 466

int melodies1[] = {
  NOTE_D_SHARP, NOTE_C_SHARP, NOTE_C_SHARP, NOTE_F_SHARP, NOTE_F_SHARP, NOTE_G_SHARP, NOTE_A, NOTE_C_SHARP,
  NOTE_C_SHARP, NOTE_D_SHARP, NOTE_C_SHARP, NOTE_D_SHARP, NOTE_C_SHARP, NOTE_A, NOTE_G_SHARP, NOTE_F_SHARP, NOTE_A, NOTE_G_SHARP, NOTE_G_SHARP, NOTE_G_SHARP,
  NOTE_C_SHARP, NOTE_C_SHARP, NOTE_D_SHARP, NOTE_F_SHARP, NOTE_F_SHARP, NOTE_G_SHARP, NOTE_A, NOTE_G_SHARP,
  NOTE_G_SHARP, NOTE_G_SHARP, NOTE_F_SHARP, NOTE_G_SHARP, NOTE_A_SHARP, NOTE_A_SHARP, NOTE_A,
  NOTE_G_SHARP, NOTE_A_SHARP, NOTE_G_SHARP, NOTE_F_SHARP, NOTE_F_SHARP
};

int durations1[] = {
  400, 400, 400, 400, 400, 400, 400, 800,                      // First line (Jeeta tha jiske liye)
  400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400,  // Second line (jiske liye marta tha)
  400, 400, 400, 400, 400, 400, 400, 800,                      // Third line (Ek aisi ladki thi)
  400, 400, 400, 400, 400, 400, 400,                           // Fourth line (jise main pyaar)
  400, 400, 400, 400                                           // Fifth line (karta tha)
};

int pardesi[][2] = {
  {NOTE_A_SHARP, 500}, {NOTE_G_SHARP, 250}, {NOTE_F_SHARP, 500}, {NOTE_A_SHARP, 250}, {NOTE_G_SHARP, 500}, {NOTE_F_SHARP, 250},
  {NOTE_F_SHARP, 500}, {NOTE_G_SHARP, 250}, {NOTE_F_SHARP, 500}, {NOTE_D_SHARP, 500},
  {NOTE_A_SHARP, 500}, {NOTE_G_SHARP, 250}, {NOTE_F_SHARP, 500}, {NOTE_A_SHARP, 250}, {NOTE_G_SHARP, 500}, {NOTE_F_SHARP, 250},
  {NOTE_F_SHARP, 500}, {NOTE_G_SHARP, 250}, {NOTE_F_SHARP, 500}, {NOTE_D_SHARP, 500},
  {NOTE_D_SHARP, 500}, {NOTE_F_SHARP, 250}, {NOTE_A_SHARP, 250}, {NOTE_G_SHARP, 250}, {NOTE_G_SHARP, 250}, {NOTE_A_SHARP, 250}, {NOTE_F_SHARP, 250}, {NOTE_G_SHARP, 250}, {NOTE_A_SHARP, 250},
  {NOTE_A_SHARP, 500}, {NOTE_A_SHARP, 250}, {NOTE_G_SHARP, 500}, {NOTE_F_SHARP, 250}, {NOTE_F_SHARP, 500},
  {NOTE_C_SHARP, 500}, {NOTE_A_SHARP, 250}, {NOTE_C_SHARP, 500}, {NOTE_C_SHARP, 500}, {NOTE_C_SHARP, 250}, {NOTE_D_SHARP, 500},
  {NOTE_C_SHARP, 500}, {NOTE_D_SHARP, 250}, {NOTE_C_SHARP, 500}, {NOTE_A_SHARP, 500}, {NOTE_A_SHARP, 250},
  {NOTE_A_SHARP, 500}, {NOTE_C_SHARP, 500}, {NOTE_C_SHARP, 500}, {NOTE_C_SHARP, 500}, {NOTE_D_SHARP, 500},
  {NOTE_C_SHARP, 500}, {NOTE_D_SHARP, 250}, {NOTE_E, 500}, {NOTE_D_SHARP, 250}, {NOTE_C_SHARP, 250}, {NOTE_A_SHARP, 500}, {NOTE_C_SHARP, 250}, {NOTE_A_SHARP, 500}, {NOTE_G_SHARP, 250}, {NOTE_F_SHARP, 250}
};

void setup() {
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  int melodyLength = sizeof(pardesi) / sizeof(pardesi[0]);  // Calculate the number of notes
  for (int i = 0; i < melodyLength; i++) {
    tone(buzzerPin, pardesi[i][0], pardesi[i][1]);
    delay(pardesi[i][1] + 50);
  }
  delay(2000);
}
