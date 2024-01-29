/*
===============================================================================
File: ESP_Python_TCP_Client.ino
Date: [29.01.2024]

Description:
   This program successfully connects to a Wi-Fi network and then establishes 
a TCP connection to a server with a specified IP address and port number. 
It subsequently sends the text "Hello World" to the server and closes the 
connection. If the connection cannot be established, it prints the message 
"Connection failed" to the serial monitor. The program repeats these steps 
within a loop after waiting for a specified duration. 

Author's Introduction:
  - Name: Sümeyye Derelli
  - Occupation: Embedded Software Developer
  - Contact: smyydrll@gmail.com
  
===============================================================================
*/





#include <WiFi.h>
#include <WiFiClient.h>

const char *ssid = "SSID";              // Yout WiFi network name / SSID
const char *password = "password";      // Your WiFi Password
const char *serverIP = "111.111.1.111"; // IP address of the host where TCPServer.py is running
const int serverPort = 80;              // Port number that TCPServer.py is listening on

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Connect to WiFi Network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  // Connect to TCP server
  WiFiClient client;
  if (client.connect(serverIP, serverPort)) {
    Serial.println("Connected to server");

    // Send to string data
    client.println("Hello World");

    // Close the connet
    client.stop();
    Serial.println("Connection closed");
  } else {
    Serial.println("Connection failed");
  }

  // Wait for 100 milisecond, then try again
  delay(100);
}
