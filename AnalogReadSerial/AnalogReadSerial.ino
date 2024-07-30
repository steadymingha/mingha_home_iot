



// /*
//  * SendDemo.cpp
//  *
//  * Demonstrates sending IR codes in standard format with address and command
//  *
//  *  This file is part of Arduino-IRremote https://github.com/Arduino-IRremote/Arduino-IRremote.
//  *
//  ************************************************************************************
//  * MIT License
//  *
//  * Copyright (c) 2020-2023 Armin Joachimsmeyer
//  *
//  * Permission is hereby granted, free of charge, to any person obtaining a copy
//  * of this software and associated documentation files (the "Software"), to deal
//  * in the Software without restriction, including without limitation the rights
//  * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  * copies of the Software, and to permit persons to whom the Software is furnished
//  * to do so, subject to the following conditions:
//  *
//  * The above copyright notice and this permission notice shall be included in all
//  * copies or substantial portions of the Software.
//  *
//  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//  * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
//  * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//  * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
//  * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
//  * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//  *
//  ************************************************************************************
//  */

// #include <Arduino.h>
// #include "PinDefinitionsAndMore.h"
// #include <IRremote.hpp>

// #define DELAY_AFTER_SEND 2000

// void setup() {
//     Serial.begin(115200);
//     while (!Serial)
//         ; // Wait for Serial to become available. Is optimized away for some cores.

// #if defined(__AVR_ATmega32U4__) || defined(SERIAL_PORT_USBVIRTUAL) || defined(SERIAL_USB) /*stm32duino*/|| defined(USBCON) /*STM32_stm32*/ \
//     || defined(SERIALUSB_PID)  || defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_attiny3217)
//     delay(4000); // To be able to connect Serial monitor after reset or power up and before first print out. Do not wait for an attached Serial Monitor!
// #endif
//     // Just to know which program is running on my Arduino
//     Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));

// #if defined(IR_SEND_PIN)
//     IrSender.begin(); // Start with IR_SEND_PIN -which is defined in PinDefinitionsAndMore.h- as send pin and enable feedback LED at default feedback LED pin
// #  if defined(IR_SEND_PIN_STRING)
//     Serial.println(F("Send IR signals at pin " IR_SEND_PIN_STRING));
// #  else
//     Serial.println(F("Send IR signals at pin " STR(IR_SEND_PIN)));
// #  endif
// #else
//     // Here the macro IR_SEND_PIN is not defined or undefined above with #undef IR_SEND_PIN
//     uint8_t tSendPin = 5;
//     IrSender.begin(tSendPin, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN); // Specify send pin and enable feedback LED at default feedback LED pin
//     // You can change send pin later with IrSender.setSendPin();

//     Serial.print(F("Send IR signals at pin "));
//     Serial.println(tSendPin);
// #endif

// #if !defined(SEND_PWM_BY_TIMER)
//     /*
//      * Print internal software PWM signal generation info
//      */
//     IrSender.enableIROut(38); // Call it with 38 kHz just to initialize the values printed below
//     Serial.print(F("Send signal mark duration is "));
//     Serial.print(IrSender.periodOnTimeMicros);
//     Serial.print(F(" us, pulse narrowing correction is "));
//     Serial.print(IrSender.getPulseCorrectionNanos());
//     Serial.print(F(" ns, total period is "));
//     Serial.print(IrSender.periodTimeMicros);
//     Serial.println(F(" us"));
// #endif
// }

// // Define the raw data as given
// uint32_t tRawData[] = {0x126CB23, 0xF032400, 0xD, 0x5800};
// const uint16_t bits = 112;
// const uint8_t protocol = PROTOCOL_IS_LSB_FIRST;
// const uint16_t repeatPeriodMillis = 0; // Set your repeat period
// const uint8_t numberOfRepeats = 0; // Set your number of repeats

// void loop() {
//     Serial.println(F("Sending PulseDistanceWidthFromArray IR signal..."));
//     IrSender.sendPulseDistanceWidthFromArray(38, // IR carrier frequency in kHz
//                                              3300, // Header mark
//                                              1600, // Header space
//                                              500, // Bit mark
//                                              1250, // One space
//                                              500, // Bit mark (zero space)
//                                              400, // Bit space
//                                              &tRawData[0], // Raw data array
//                                              bits, // Number of bits
//                                              protocol, // Protocol (LSB first)
//                                              repeatPeriodMillis, // Repeat period
//                                              numberOfRepeats); // Number of repeats
//     delay(DELAY_AFTER_SEND);
// }
