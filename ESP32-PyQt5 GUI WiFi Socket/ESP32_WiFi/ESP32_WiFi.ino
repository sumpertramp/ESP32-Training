/*
===============================================================================
File: Test_ESP32_TCP_Connection.ino
Date: [13.12.2023]

Description:
   This code is designed for systems where you need to handle incoming client 
connections concurrently. It establishes a connection to a WiFi network and 
a TCP server on ESP32.  

Author's Introduction:
  - Name: Sümeyye Derelli
  - Occupation: Embedded Software Developer
  - Contact: smyydrll@gmail.com
  
===============================================================================
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

const char *ssid = " WiFi SSID";
const char *password = "WiFi Password";

WiFiServer server(80);  // TCP server port

void setup() {
  Serial.begin(115200);

  // Initialize WiFi connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connection successful");
  Serial.println(WiFi.localIP());
  
  // Start the TCP server
  server.begin();
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connection established");
  }
}

void loop() {
    // Read data from the client
    WiFiClient client = server.available();
    if (client) {
      Serial.println("New client connection established");
    }
    while (client.connected()) {
      if (client.available()) {
        String request = client.readStringUntil('\r');
        Serial.println("Received data: " + request);

        // Send a response to the client
        client.println("Hello, response from ESP32");

        // Close the client connection
        client.stop();
        Serial.println("Client connection closed");
      }
    }
}

