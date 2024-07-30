#include "WiFiEsp.h"
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
    delay(10000);
  }

  Serial.println("You're connected to the network");
  printWifiStatus();

  // Start the server
  server.begin();
}

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
          ledStatus = HIGH;
          // digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
        }
        else if (buf.endsWith("GET /L")) {
          Serial.println("Turn led OFF");
          ledStatus = LOW;
          // digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
        }
      }
    }
    
    // close the connection
    client.stop();
    Serial.println("Client disconnected");
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