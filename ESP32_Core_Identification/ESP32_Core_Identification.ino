/*
===============================================================================
File: ESP32_Core_Identification.ino
Date: [08.12.2023]

Description:
   This code utilizes the xPortGetCoreID() function to determine the core on which 
both the setup() and loop() functions are running. Subsequently, it displays this 
information on the Serial monitor. This enables tracking of which core the program 
is operating on, facilitating the ability to monitor and, if necessary, balance or 
optimize tasks among different cores. 

Author's Introduction:
 -Name: Sümeyye Derelli
 -Occupation: Embedded Software Developer
 -Contact: smyydrll@gmail.com
 
===============================================================================
*/

void setup() {
  Serial.begin(115200);
  Serial.print("setup() running on this core: ");
  Serial.println(xPortGetCoreID());
}

void loop() {
  Serial.print("loop() running on this core: ");
  Serial.println(xPortGetCoreID()); 
}
