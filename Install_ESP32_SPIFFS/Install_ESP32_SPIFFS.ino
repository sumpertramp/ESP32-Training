/*
===============================================================================
File: Test_ESP32_TCP_Connection.ino
Date: [10.01.2024]
 
Description:
* Download ESP32FS-1.1.zip --> https://github.com/me-no-dev/arduino-esp32fs-plugin/releases/
* Find your Sketchbook location in Arduino IDE. --> File --> Preferences
* Tools --> ESP32 Sketch Data Upload
* Sketch --> Show Sketch Folder --> Inside folder of data. 
* What do you want save into the ESP32 save the data folder.  

Author's Introduction:
  - Name: Sümeyye Derelli
  - Occupation: Embedded Software Developer
  - Contact: smyydrll@gmail.com
  
===============================================================================
*/
#include "SPIFFS.h"
 
void setup() {
  Serial.begin(115200);
  
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  File file = SPIFFS.open("/test.txt");
  if(!file){
    Serial.println("Failed to open file for reading");
    return;
  }
  
  Serial.println("File Content:");
  while(file.available()){
    Serial.write(file.read());
  }
  file.close();
}
 
void loop() {

}
