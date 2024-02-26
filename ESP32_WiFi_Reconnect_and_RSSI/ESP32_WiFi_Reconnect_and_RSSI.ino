/*
===============================================================================
File: Test_ESP32_TCP_Connection.ino
Date: [10.01.2024]

Description:
   This code sets up an ESP32 microcontroller as a Station (STA) that connects
   to a specified WiFi network using the provided SSID and password. It includes
   a function to initialize the WiFi connection and a loop to periodically check
   and reconnect if the connection is lost.

Author's Introduction:
  - Name: Sümeyye Derelli
  - Occupation: Embedded Software Developer
  - Contact: smyydrll@gmail.com

===============================================================================
*/

#include <WiFi.h>

// Replace with your network credentials (STATION)
const char* ssid = "WiFi SSID";
const char* password = "WiFi Password";

unsigned long previousMillis = 0;
unsigned long interval = 30000; // Interval to check WiFi connection (30 seconds)

// Function to initialize WiFi connection
void initWiFi() {
  WiFi.mode(WIFI_STA); // Set WiFi mode to station (client)
  WiFi.begin(ssid, password); // Connect to WiFi network with provided credentials
  Serial.print("Connecting to WiFi ..");
  
  // Wait for WiFi connection
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.'); // Print dots while waiting
    delay(1000);
  }
  Serial.println(WiFi.localIP()); // Print local IP address once connected
}

void setup() {
  Serial.begin(115200); // Initialize serial communication at 115200 baud
  initWiFi(); // Initialize WiFi connection
  
  // Print WiFi signal strength (RSSI)
  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());
}

void loop() {
  unsigned long currentMillis = millis(); // Get current time in milliseconds
  
  // If WiFi is disconnected and enough time has passed, attempt to reconnect
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >= interval)) {
    Serial.print(millis());
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect(); // Disconnect from current network
    WiFi.reconnect(); // Attempt to reconnect
    previousMillis = currentMillis; // Update last reconnect time
  }
}
