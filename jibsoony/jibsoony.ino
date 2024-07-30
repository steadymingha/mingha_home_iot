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


void loop()
{
  WiFiEspClient client = server.available();  

  if (client) {                               
    Serial.println("New client");             
    buf.init();                               
    while (client.connected()) {              
      if (client.available()) {               
        char c = client.read();               
        buf.push(c);                          

        if (buf.endsWith("\r\n\r\n")) {
          sendHttpResponse(client);
          break;
        }

        if (buf.endsWith("GET /H")) {
          tRawData[0] = 0x126CB23;
          tRawData[1] = 0x32400;
          tRawData[2] = 0xA;
          tRawData[3] = 0x4600;
          flag = true;
          
        } else if (buf.endsWith("GET /L")) {
          tRawData[0] = 0x126CB23;
          tRawData[1] = 0xF032000;
          tRawData[2] = 0xD;
          tRawData[3] = 0x5400;
          flag = true;

        } else if (buf.endsWith("GET /U")) {
          if (tRawData[1] > 0x32400) {
            tRawData[1] -= 0x1000000;
            tRawData[3] -= 0x0100;
            flag = true;
          }

        } else if (buf.endsWith("GET /D")) {
          if (tRawData[1] < 0xF032400) {
            tRawData[1] += 0x1000000;
            tRawData[3] += 0x0100;
            flag = true;
          }
        }
      }
    }
    
    client.stop();
    Serial.println("Client disconnected");
  }

  if (flag==true) {
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
  
  // The HTTP response ends with another blank line:
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


