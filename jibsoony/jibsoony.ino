#include "WiFiEsp.h"
#include <Arduino.h>
#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp>
#include "SoftwareSerial.h"

// 소프트웨어 시리얼 포트 초기화
SoftwareSerial EspSerial(2, 3); // RX, TX

char ssid[] = "spaceshuttle";     // your network SSID (name)
char pass[] = "!dlaudghk88";      // your network password
int status = WL_IDLE_STATUS;      // the Wifi radio's status
int ledStatus = LOW;
WiFiEspServer server(80);
RingBuffer buf(8);
void setup()
{
  // Initialize serial for debugging
  Serial.begin(115200);


  IrSender.begin(); // Start with IR_SEND_PIN -which is defined in PinDefinitionsAndMore.h- as send pin and enable feedback LED at default feedback LED pin
    // Serial.println(F("Send IR signals at pin " STR(IR_SEND_PIN)));
  IrSender.enableIROut(38); // Call it with 38 kHz just to initialize the values printed below


  // Initialize serial for ESP module
  EspSerial.begin(9600);
  // Initialize ESP module
  WiFi.init(&EspSerial);

  // Check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true); // don't continue
  }

  // Attempt to connect to WiFi network
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
    delay(5000);
  }

  Serial.println("You're connected to the network");
  printWifiStatus();

  // Start the server
  server.begin();
}

// Define the raw data as given
uint32_t tRawData[] = {0x126CB23, 0x32000, 0x4A, 0x9412};
uint16_t header_seq[7] = {0};
const uint16_t bits = 112;
const uint8_t protocol = PROTOCOL_IS_LSB_FIRST;
const uint16_t repeatPeriodMillis = 0; // Set your repeat period
const uint8_t numberOfRepeats = 0; // Set your number of repeats
bool flag = false;

void loop()
{
  WiFiEspClient client = server.available();  // listen for incoming clients

  if (client) {                               // if you get a client,
    Serial.println("New client");             // print a message out the serial port
    buf.init();                               // initialize the circular buffer
    while (client.connected()) {              // loop while the client's connected
      if (client.available()) {               // if there's bytes to read from the client,
        char c = client.read();               // read a byte, then
        buf.push(c);                          // push it to the ring buffer

        // printing the stream to the serial monitor will slow down
        // the receiving of data from the ESP filling the serial buffer
        //Serial.write(c);
        
        // you got two newline characters in a row
        // that's the end of the HTTP request, so send a response
        if (buf.endsWith("\r\n\r\n")) {
          sendHttpResponse(client);
          break;
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (buf.endsWith("GET /H")) {
          Serial.println("Turn led ON");
          tRawData[0] = 0x126CB23;
          tRawData[1] = 0x4032400;
          tRawData[2] = 0xA;
          tRawData[3] = 0x4A00;

          header_seq[0] = 38;
          header_seq[1] = 3350;
          header_seq[2] = 1600;
          header_seq[3] = 500;
          header_seq[4] = 1250;
          header_seq[5] = 500;
          header_seq[6] = 400;
          flag = true;
          
        //   IrSender.sendPulseDistanceWidthFromArray(38, 3350, 1600, 500, 1250, 500, 400, &tRawData[0], bits, protocol, repeatPeriodMillis, numberOfRepeats);    
          // ledStatus = HIGH;
          // digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
        }
        else if (buf.endsWith("GET /L")) {
          Serial.println("Turn led OFF");
          tRawData[0] = 0x126CB23;
          tRawData[1] = 0xF032000;
          tRawData[2] = 0xD;
          tRawData[3] = 0x5400;

          header_seq[0] = 38;
          header_seq[1] = 3300;
          header_seq[2] = 1650;
          header_seq[3] = 500;
          header_seq[4] = 1250;
          header_seq[5] = 500;
          header_seq[6] = 400;
          flag = true;
          // IrSender.sendPulseDistanceWidthFromArray(38, 3300, 1650, 500, 1250, 500, 400, &tRawData[0], bits, protocol, repeatPeriodMillis, numberOfRepeats);

          // ledStatus = LOW;
          // digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
        }
      }
    }
    
    // close the connection
    client.stop();
    Serial.println("Client disconnected");
  }

  if (flag==true)
  {
  IrSender.sendPulseDistanceWidthFromArray(header_seq[0], header_seq[1], header_seq[2], header_seq[3], header_seq[4], header_seq[5], header_seq[6], &tRawData[0], bits, protocol, repeatPeriodMillis, numberOfRepeats);
  flag = false;
  }



}
void sendHttpResponse(WiFiEspClient client)
{
  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();
  
  // // the content of the HTTP response follows the header:
  // client.print("The LED is ");
 
  // client.println("<br>");
  // client.println("<br>");
  
  // client.println("Click <a href=\"/H\">here</a> turn the LED on<br>");
  // client.println("Click <a href=\"/L\">here</a> turn the LED off<br>");
  
  // The HTTP response ends with another blank line:
  client.println();
}

void printWifiStatus()
{
  // Print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // Print your board's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // Print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}



// #include <SoftwareSerial.h>

// SoftwareSerial mySerial(2, 3); // RX, TX

// void setup() {
//   Serial.begin(57600);
//   mySerial.begin(57600);
// }

// void loop() {
//   if (mySerial.available()) {
//     Serial.write(mySerial.read());
//   }
//   if (Serial.available()) {
//     mySerial.write(Serial.read());
//   }
// }



// AT+CWMODE?

// +CWMODE:9

// OK
// AT+CWMODE=3


// OK
// AT+CWJAP?

// No AP

// OK
// AT+CWJAP="spaceshuttle","!dlaudghk88"

//AT+UART_DEF=9600,8,1,0,0

// WIFI CONNECTED
// WIFI GOT IP

// OK
// AT+CWJAP?

// +CWJAP:"spaceshuttle","88:36:6c:39:6c:ce",13,-29

// OK