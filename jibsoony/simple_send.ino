// #include <Arduino.h>
// #include "PinDefinitionsAndMore.h"
// #include <IRremote.hpp>
// #define IR_SEND_PIN         5   // GPIO16
// #define DELAY_AFTER_SEND 2000

// void setup() {
//     Serial.begin(115200);
//     while (!Serial)
//     ;


//     // Just to know which program is running on my Arduino
//     Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

//     IrSender.begin(); // Start with IR_SEND_PIN -which is defined in PinDefinitionsAndMore.h- as send pin and enable feedback LED at default feedback LED pin
//     // Serial.println(F("Send IR signals at pin " STR(IR_SEND_PIN)));


//     IrSender.enableIROut(38); // Call it with 38 kHz just to initialize the values printed below

// }

// // Define the raw data as given
// uint32_t tRawData[] = {0x126CB23, 0x32000, 0x4A, 0x9412};
// const uint16_t bits = 112;
// const uint8_t protocol = PROTOCOL_IS_LSB_FIRST;
// const uint16_t repeatPeriodMillis = 0; // Set your repeat period
// const uint8_t numberOfRepeats = 0; // Set your number of repeats

// void loop() {
//     Serial.println(F("Sending PulseDistanceWidthFromArray IR signal..."));
//     tRawData[0] = 0x126CB23;
//     tRawData[1] = 0x32400;
//     tRawData[2] = 0x4A;
//     tRawData[3] = 0x9812;
//     IrSender.sendPulseDistanceWidthFromArray(38, 3350, 1650, 450, 1150, 450, 400, &tRawData[0], bits, protocol, repeatPeriodMillis, numberOfRepeats);
//     // IrSender.sendPulseDistanceWidthFromArray(38, // IR carrier frequency in kHz
//     //                                          3400, // Header mark
//     //                                          1650, // Header space
//     //                                          400, // Bit mark
//     //                                          1200, // One space
//     //                                          400, // Bit mark (zero space)
//     //                                          450, // Bit space
//     //                                          &tRawData[0], // Raw data array
//     //                                          bits, // Number of bits
//     //                                          protocol, // Protocol (LSB first)
//     //                                          repeatPeriodMillis, // Repeat period
//     //                                          numberOfRepeats); // Number of repeats
//     delay(DELAY_AFTER_SEND);
// }