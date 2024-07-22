#include "WiFiEsp.h"
#include "SoftwareSerial.h"

// 소프트웨어 시리얼 포트 초기화
SoftwareSerial EspSerial(2, 3); // RX, TX

char ssid[] = "spaceshuttle";     // your network SSID (name)
char pass[] = "!dlaudghk88";      // your network password
int status = WL_IDLE_STATUS;      // the Wifi radio's status

WiFiEspServer server(80);

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
  // Listen for incoming clients
  WiFiEspClient client = server.available();
  if (client) {
    Serial.println("New client connected");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<h1>Hello from Arduino!</h1>");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the client time to receive the data
    delay(10);
    // close the connection
    client.stop();
    Serial.println("Client disconnected");
  }
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