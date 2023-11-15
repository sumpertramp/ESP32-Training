/*
===============================================================================
File: Test_ESP32_SPIFFS.ino
Date: [14.11.2023]

Description:
  This Arduino code is designed for the ESP32 microcontroller to demonstrate
  the usage of SPIFFS (Serial Peripheral Interface Flash File System). SPIFFS
  allows storing and accessing files on the ESP32's flash memory. In this test,
  the code initializes SPIFFS, writes a sample text file, and reads its content.

  Author's Introduction:
  - Name: Sümeyye Derelli
  - Occupation: smyydrll@gmail.com
  - Contact: Embedded Software Developer

  Notes:
  - Make sure to update the SPIFFS settings as needed.
===============================================================================
*/

#include "FS.h"
#include "SPIFFS.h"

//Formatting option when initializing SPIFFS file system
#define FORMAT_SPIFFS_IF_FAILED true

//List the specified SPIFFS file directory and subdirectories up to the specifield level
void listDir(fs::FS &fs, const char *dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

//Reads the specified file and displays its contents via serial port
void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return;
    }

    Serial.println("- read from file:");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}

//writes the specified message to the specified file
void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("- file written");
    } else {
        Serial.println("- write failed");
    }
    file.close();
}

void setup(){
    Serial.begin(115200);

    //SPIFFS file system is initialized, error message is shown if formatting fails
    if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
        Serial.println("SPIFFS Mount Failed");
        return;
    }

    //List the main file directory and its contents
    listDir(SPIFFS, "/", 0);

    //Creates the test file and displays its contents
    writeFile(SPIFFS, "/YourFile.txt", "This file is to test if the microcontroller can read it....! ");// This is file name and its contents...
    readFile(SPIFFS, "/YourFile.txt");
}

void loop(){
}
