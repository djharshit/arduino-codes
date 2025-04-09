#include <IRremote.hpp>

#define DECODE_DENON
#define DECODE_JVC
#define DECODE_KASEIKYO
#define DECODE_PANASONIC
#define DECODE_LG
#define DECODE_NEC
#define DECODE_SAMSUNG
#define DECODE_SONY
#define DECODE_RC5
#define DECODE_RC6
#define DECODE_BOSEWAVE
#define DECODE_LEGO_PF
#define DECODE_MAGIQUEST
#define DECODE_WHYNTER
#define DECODE_FAST
#define DECODE_DISTANCE_WIDTH
#define DECODE_HASH
#define DECODE_BEO
#define DEBUG
#define RAW_BUFFER_LENGTH 750

#include <IRremote.hpp>
#include <Servo.h>

#define IR_RECEIVE 7

Servo myServo;

void setup() {
  // Serial.begin(9600);
  // Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));
  IrReceiver.begin(IR_RECEIVE, ENABLE_LED_FEEDBACK);
  // Serial.print(F("Ready to receive IR signals of protocols: "));
  // printActiveIRProtocols(&Serial);
  // Serial.print(F("at pin "));
  // Serial.println(IR_RECEIVE);

  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  myServo.attach(1);
}

void loop() {
  if (IrReceiver.decode()) {
    if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
      // Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
      // IrReceiver.printIRResultRawFormatted(&Serial, true);
      IrReceiver.resume();
    } else {
      IrReceiver.resume();
      // IrReceiver.printIRResultShort(&Serial);
      // IrReceiver.printIRSendUsage(&Serial);
    }
    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
      // Serial.println(F("Repeat received. Here you can repeat the same action as before."));
    } else {
      if (IrReceiver.decodedIRData.command == 0x1) {
        // Serial.println("open");
        myServo.write(165);
      }
      else if (IrReceiver.decodedIRData.command == 0x2) {
        // Serial.println("close");
        myServo.write(180);
      }
      else if (IrReceiver.decodedIRData.command == 0x2) {
        // Serial.println("close");
        tone(0, 1000);
        delay(1000);
        noTone(0);
      }
    }
  }
}
