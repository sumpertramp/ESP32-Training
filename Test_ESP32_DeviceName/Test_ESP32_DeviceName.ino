/*
===============================================================================
File: Test_ESP32_DeviceName.ino
Date: [29.11.2023]

Description:
  This Arduino code is developed for the ESP32 microcontroller to showcase the
  generation of a unique device name based on its WiFi MAC address. The device
  name is formatted as "ESP32_XX:XX:XX," where XX represents the last three
  bytes of the MAC address. The generated device name is then printed to the
  serial monitor.

Author's Introduction:
  - Name: Sümeyye Derelli
  - Occupation: Embedded Software Developer
  - Contact: smyydrll@gmail.com

Notes:
  - Ensure that the ESP32 WiFi settings are properly configured.
===============================================================================
*/

#include <WiFi.h>

String generateDeviceName() {
  uint8_t baseMac[6];
  esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
  char baseMacChr[18] = {0};
  sprintf(baseMacChr, "ESP32_%02X%02X%02X", baseMac[3], baseMac[4], baseMac[5]);
  return String(baseMacChr);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();
  
  String deviceName = generateDeviceName();
  Serial.print("ESP32 device name is: ");
  Serial.println(deviceName);
  Serial.print("ESP32 device name is: ");
  Serial.println(deviceName); 
  
}

void loop() {
  // put your main code here, to run repeatedly:  
}
