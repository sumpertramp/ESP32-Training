/*
===============================================================================
File: ESP32_PWM.ino
Date: [04.03.2024]

Description:
   This code initializes and controls the brightness of an LED using PWM on an
   ESP32 microcontroller. The LED is connected to GPIO pin 27. The code gradually
   increases and decreases the LED brightness in a loop, creating a fading effect.

Author's Introduction:
  - Name: Sümeyye Derelli
  - Occupation: Embedded Software Developer
  - Contact: smyydrll@gmail.com
  
===============================================================================
*/

// the number of the LED pin
const int ledPin = 27;  // 27 corresponds to GPIO27

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;
 
void setup() {
  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);
}
 
void loop() {
  // increase the LED brightness
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++) {
    // changing the LED brightness with PWM
    ledcWrite(ledChannel, dutyCycle);
    delay(15);
  }

  // decrease the LED brightness
  for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--) {
    // changing the LED brightness with PWM
    ledcWrite(ledChannel, dutyCycle);   
    delay(15);
  }
}
