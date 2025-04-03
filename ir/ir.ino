#include <IRremote.hpp>
#include <ESP32Servo.h>

/*
 * Specify which protocol(s) should be used for decoding.
 * If no protocol is defined, all protocols (except Bang&Olufsen) are active.
 * This must be done before the #include <IRremote.hpp>
 */
#define DECODE_DENON        // Includes Sharp
#define DECODE_JVC
#define DECODE_KASEIKYO
#define DECODE_PANASONIC    // alias for DECODE_KASEIKYO
#define DECODE_LG
#define DECODE_NEC          // Includes Apple and Onkyo. To enable all protocols , just comment/disable this line.
#define DECODE_SAMSUNG
#define DECODE_SONY
#define DECODE_RC5
#define DECODE_RC6

#define DECODE_BOSEWAVE
#define DECODE_LEGO_PF
#define DECODE_MAGIQUEST
#define DECODE_WHYNTER
#define DECODE_FAST

#define DECODE_DISTANCE_WIDTH // Universal decoder for pulse distance width protocols
#define DECODE_HASH         // special decoder for all protocols

#define DECODE_BEO          // This protocol must always be enabled manually, i.e. it is NOT enabled if no protocol is defined. It prevents decoding of SONY!

#define DEBUG               // Activate this for lots of lovely debug output from the decoders.

#define RAW_BUFFER_LENGTH  750 // For air condition remotes it may require up to 750. Default is 200.

/*
 * This include defines the actual pin number for pins like IR_RECEIVE_PIN, IR_SEND_PIN for many different boards and architectures
 */

#include <IRremote.hpp> // include the library

Servo myservo;

const int IR_RECEIVE_PIN = 13;

void setup() {
    Serial.begin(9600);
    myservo.attach(18);

    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

    // Start the receiver and if not 3. parameter specified, take LED_BUILTIN pin from the internal boards definition as default feedback LED
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

    Serial.print(F("Ready to receive IR signals of protocols: "));
    printActiveIRProtocols(&Serial);
    Serial.print(F("at pin "));
    Serial.print(IR_RECEIVE_PIN);
}

void loop() {
    /*
     * Check if received data is available and if yes, try to decode it.
     * Decoded result is in the IrReceiver.decodedIRData structure.
     *
     * E.g. command is in IrReceiver.decodedIRData.command
     * address is in command is in IrReceiver.decodedIRData.address
     * and up to 32 bit raw data in IrReceiver.decodedIRData.decodedRawData
     */
    if (IrReceiver.decode()) {

        /*
         * Print a summary of received data
         */
        if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
            Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
            // We have an unknown protocol here, print extended info
            IrReceiver.printIRResultRawFormatted(&Serial, true);

            IrReceiver.resume(); // Do it here, to preserve raw data for printing with printIRResultRawFormatted()
        } else {
            IrReceiver.resume(); // Early enable receiving of the next IR frame

            IrReceiver.printIRResultShort(&Serial);
            IrReceiver.printIRSendUsage(&Serial);
        }
        Serial.println();

        /*
         * Finally, check the received data and perform actions according to the received command
         */
        if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
            Serial.println(F("Repeat received. Here you can repeat the same action as before."));
        } else {
            if (IrReceiver.decodedIRData.command == 0x8) {
                Serial.println(F("Received command 0x8."));
                myservo.write(0);
            }
            else if (IrReceiver.decodedIRData.command == 0xC) {
                Serial.println(F("Received command 0xC."));
                myservo.write(45);
            }
            else if (IrReceiver.decodedIRData.command == 0x18) {
                Serial.println(F("Received command 0x18."));
                myservo.write(90);
            }
            else if (IrReceiver.decodedIRData.command == 0x5A) {
                Serial.println(F("Received command 0x5A."));
                myservo.write(180);
            }
            else if (IrReceiver.decodedIRData.command == 0x5E) {
                Serial.println(F("Received command 0x5E."));
                myservo.write(135);
            }
        }
    }
}
