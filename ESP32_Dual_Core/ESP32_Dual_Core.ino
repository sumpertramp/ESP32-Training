/*
===============================================================================
File: Test_ESP32_DeviceName.ino
Date: [29.11.2023]

Description:
   This code can be used in systems where you want to perform two different tasks 
concurrently. Both tasks cause an LED to blink at specific time intervals on 
ESP32.
  

Author's Introduction:
  - Name: Sümeyye Derelli
  - Occupation: Embedded Software Developer
  - Contact: smyydrll@gmail.com
  
===============================================================================
*/


TaskHandle_t Task1;
TaskHandle_t Task2;

// LED pins
const int ledPin1 = 27;
const int ledPin2 = 26;

void setup() {
  Serial.begin(115200); 
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);

  //create a task that will be executed in the Task1code() function, with priority
  //1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   
                    "Task1",     
                    10000,       
                    NULL,      
                    1,           
                    &Task1,      
                    0);                         
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 
  //1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   
                    "Task2",     
                    10000,       
                    NULL,        
                    1,           
                    &Task2,      
                    1);          
    delay(500); 
}

//Task1code: blinks an LED every 1000 ms
void Task1code( void * pvParameters ){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    digitalWrite(ledPin1, HIGH);
    delay(1000);
    digitalWrite(ledPin1, LOW);
    delay(1000);
  } 
}

//Task2code: blinks an LED every 700 ms
void Task2code( void * pvParameters ){
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    digitalWrite(ledPin2, HIGH);
    delay(500);
    digitalWrite(ledPin2, LOW);
    delay(500);
  }
}

void loop() {
  
}
  


