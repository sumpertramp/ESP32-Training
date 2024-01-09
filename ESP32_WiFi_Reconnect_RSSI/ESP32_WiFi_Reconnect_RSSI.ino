/*
===============================================================================
File: ESP32_WiFi_Reconnect_RSSI.ino
Date: [09.01.2024]

Description:
   This code is designed for ESP32 and handles WiFi connection initialization 
and maintenance. It primarily attempts to connect to WiFi for 10 seconds. If the 
connection is lost, it attempts to reconnect at regular intervals. The status is 
printed to the serial monitor, including the local IP address and WiFi signal 
strength (RSSI).
  
Author's Introduction:
  - Name: Sümeyye Derelli
  - Occupation: Embedded Software Developer
  - Contact: smyydrll@gmail.com
    
===============================================================================
*/

#include <WiFi.h>

// Enter your network information
const char* ssid = "Sumeyye";
const char* password = "sum183542";

unsigned long previousMillis = 0;
unsigned long interval = 30000;  // Interval for checking WiFi connection in milliseconds

// Function to initialize WiFi connection
void initWiFi() {
  unsigned long startTime = millis();  // Save the start time
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
    WiFi.begin(ssid, password);  // Try to connect to the WiFi
    Serial.print("Connecting to WiFi...");
    delay(1000);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to WiFi");
    Serial.print("RSSI: ");
    Serial.println(WiFi.RSSI());  // Print the WiFi signal strength
  } else {
    Serial.println("\nFailed to connect to WiFi. Please check your network credentials and try again.");
  }
}

void setup() {
  Serial.begin(115200);  // Initialize serial communication
  initWiFi();  // Initialize WiFi connection
}

void loop() {
  unsigned long currentMillis = millis();
  
  // If the WiFi connection is lost, attempt to reconnect every CHECK_WIFI_TIME seconds
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >= interval)) {
    Serial.print(currentMillis);
    Serial.println(" Reconnecting to WiFi...");
    WiFi.disconnect();  // Terminate the current connection
    WiFi.reconnect();   // Attempt to reconnect
    previousMillis = currentMillis;
  }

  if (WiFi.status() == WL_CONNECTED){
    Serial.println("\nConnected to WiFi");
    Serial.print("RSSI: ");
    Serial.println(WiFi.RSSI());  // Print te WiFi signal strength.
  }
}
