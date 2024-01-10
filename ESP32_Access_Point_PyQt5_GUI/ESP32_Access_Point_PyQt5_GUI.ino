
/*
===============================================================================
File: Test_ESP32_TCP_Connection.ino
Date: [10.01.2024]

Description:
   This code sets up an ESP32 microcontroller as an Access Point (AP) with a 
specified SSID and password.  It creates a simple web server that listens 
on port 80 and responds with a welcome message to any incoming connection request. 

Author's Introduction:
  - Name: Sümeyye Derelli
  - Occupation: Embedded Software Developer
  - Contact: smyydrll@gmail.com
  
===============================================================================
*/
#include <WiFi.h>

const char *ssid = "ESP32-Access-Point";
const char *password = "123456789";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.print("Setting up AP (Access Point)...");

  // Configuring the Access Point
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Starting the server to listen for incoming connections
  server.begin();
}

void loop() {

  // Listening for connection requests
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New connection request received");

    // Accepting the first incoming connection
    client.println("Connection successful! Welcome.");

    // Closing the connection
    client.stop();
    Serial.println("Connection closed");
  }
}
