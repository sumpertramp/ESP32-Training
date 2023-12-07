#include "WiFi.h"

#define TXD 17
#define RXD 16

const char* ssid = "SUPERONLINE_WiFi_4561";
const char* password = "ermas0478ermas";

//Set web server pot number to 80
WiFiServer server(80);

//Variable to store the HTTP request
String header;
String STM32data;

String output27State = "off";
const int output27 = 27;
String output26State = "off";
const int output26 = 26;

unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 10;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(115200,SERIAL_8N1, RXD, TXD);
  
  pinMode(output27, OUTPUT);
  digitalWrite(output27, LOW);
  pinMode(output26, OUTPUT);
  digitalWrite(output26, LOW);

  //Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(output26, HIGH);
    delay(500);
    digitalWrite(output26, LOW);    
  }

  //Print local IP adress and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP adress: ");
  Serial.println(WiFi.localIP());
  digitalWrite(output26, HIGH);
  
  //Start the web server
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly
   WiFiClient client = server.available();   
   
  if (client) {                             
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          
    String currentLine = "";                
    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();
      if (client.available()) {             
        char c = client.read();
        Serial.write(c);                    
        header += c;
        if (c == '\n') {                    
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("start") >= 0) {
               Serial.println("GPIO 27 on");
              output27State = "on";
              digitalWrite(output27, HIGH);
            } else if (header.indexOf("stop") >= 0) {
              Serial.println("GPIO 27 off");
              output27State = "off";
              digitalWrite(output27, LOW);
            }
            client.println();
            // Break out of the while loop
            break;
          } else { 
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c; 
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
