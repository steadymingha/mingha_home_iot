#include "WiFiEsp.h"
#include <Arduino.h>
#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp>
#include "SoftwareSerial.h"

SoftwareSerial EspSerial(2, 3); 

char ssid[] = "spaceshuttle";     
char pass[] = "!dlaudghk88";      
int status = WL_IDLE_STATUS;      
WiFiEspServer server(80);
RingBuffer buf(8);

void setup()
{
  Serial.begin(115200);


  IrSender.begin(); 
  IrSender.enableIROut(38); // Call it with 38 kHz just to initialize the values printed below


  EspSerial.begin(9600);
  WiFi.init(&EspSerial);

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true); 
  }

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(5000);
  }

  Serial.println("You're connected to the network");
  printWifiStatus();

  server.begin();
}

// Define the raw data as given
uint32_t tRawData[4] = {0x126CB23, 0x32400, 0xA, 0x4600};
uint16_t header_seq[7] = {38, 3350, 1600, 400, 1250, 500, 400};
const uint16_t bits = 112;
const uint8_t protocol = PROTOCOL_IS_LSB_FIRST;
const uint16_t repeatPeriodMillis = 0; 
const uint8_t numberOfRepeats = 0; 
bool flag = false;
uint8_t startIdx = 0;
uint8_t endIdx = 0;
String tempId = "";


void loop() {
  // Check if a client has connected
  WiFiEspClient client = server.available();
  if (client) {
    Serial.println("New client connected");
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();
    
    // Match the request
    if (request.indexOf("GET /H ") >= 0) {

      tRawData[0] = 0x126CB23;
      tRawData[1] = 0x32400;
      tRawData[2] = 0xA;
      tRawData[3] = 0x4600;
      flag = true;

    } else if (request.indexOf("GET /L ") >= 0) {

      tRawData[0] = 0x126CB23;
      tRawData[1] = 0xF032000;
      tRawData[2] = 0xD;
      tRawData[3] = 0x5400;
      flag = true;

    } else if (request.startsWith("GET /temp/")) {

      startIdx = request.indexOf("/temp/") + 6;
      endIdx = request.indexOf(" ", startIdx);
      tempId = request.substring(startIdx, endIdx);
      
      tRawData[1] = 0xF032400 - (tempId.toInt()-1)*0x1000000;  //16 :1, 0xF032400  / 31: 16, 0x0032400
      tRawData[2] = 0xA;
      tRawData[3] = 0x5500 - (tempId.toInt()-1)*0x0100;       //16:1, 0x5500 / 31: 16, 0x4600
      flag = true;

    } else {
      // Unknown endpoint
      client.println("HTTP/1.1 404 Not Found");
      client.println("Content-Type: text/html");
      client.println("Connection: close");
      client.println();
      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
      client.println("<h1>404 Not Found</h1>");
      client.println("</html>");
    }
    
    
    // Close the connection
    sendResponse(client);
    client.stop();
    Serial.println("Client disconnected");
  }

  if (flag == true) {    
        IrSender.sendPulseDistanceWidthFromArray(header_seq[0], header_seq[1], header_seq[2], header_seq[3], header_seq[4], header_seq[5], header_seq[6], &tRawData[0], bits, protocol, repeatPeriodMillis, numberOfRepeats);
    flag = false;
  }
}

void sendResponse(WiFiEspClient client) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println();
}
void printWifiStatus()
{
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}


