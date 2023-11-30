/*
===============================================================================
File: Test_ESP32_WebServer.ino
Date: [30.11.2023]

Description:
  This Arduino code is designed for the ESP32 microcontroller to create a 
  simple web server. The web server allows control of GPIO pins through a 
  web interface. 

Author's Introduction:
 -Name: Sümeyye Derelli
 -Occupation: Embedded Software Developer
 -Contact: smyydrll@gmail.com
 
Notes:Ensure that the ESP32 WiFi settings are properly configured.
===============================================================================
*/

#include <WiFi.h>

const char* ssid = "SUPERONLINE_WiFi_4561";
const char* password = "ermas0478ermas";

WiFiServer server(80);

String header;

String output26State = "off";
String output27State = "off";

const int output26 = 26;
const int output27 = 27;

unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);

  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
  digitalWrite(output26, LOW);
  digitalWrite(output27, LOW);

  connectToWiFi();
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    handleClient(client);
  }
}

void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.println(".");
  }
}

void handleClient(WiFiClient client) {
  currentTime = millis();
  previousTime = currentTime;
  Serial.println("New Client.");

  String currentLine = "";

  while (client.connected() && currentTime - previousTime <= timeoutTime) {
    currentTime = millis();

    if (client.available()) {
      char c = client.read();
      Serial.write(c);
      header += c;

      if (c == '\n') {
        if (currentLine.length() == 0) {
          sendHttpResponse(client);
          break;
        }else {
          currentLine = "";
        }
      }else if (c != '\r') {
        currentLine += c;
      }
    }
  }

  header = "";
  client.stop();
  Serial.println("Client cisconnected.");
  Serial.println("");
}

void sendHttpResponse(WiFiClient client) {
  if (header.indexOf("GET /26/on") >= 0) {
    toggleOutput(output26, HIGH, "GPIO 26 on", "on", "off");
  } else if (header.indexOf("GET /26/off") >= 0) {
    toggleOutput(output26, LOW, "GPIO 26 off", "off", "on");
  } else if (header.indexOf("GET /27/on") >= 0) {
    toggleOutput(output27, HIGH, "GPIO 27 on", "on", "off");
  } else if (header.indexOf("GET /27/off") >= 0) {
    toggleOutput(output27, LOW, "GPIO 27 off", "off", "on");
  }

  sendHtmlPage(client);
  
}

void toggleOutput(int pin, int value, const char* message, const char* newState, const char* oppositeState) {
  Serial.println(message);
  digitalWrite(pin, value);
  if (pin == output26) {
    output26State = newState;
  } else if (pin == output27) {
    output27State = newState;
  }
}

void sendHtmlPage(WiFiClient client) {
  client.println("<!DOCTYPE html><html>");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");
  client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
  client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
  client.println(".button2 {background-color: #555555;}</style></head>");

  client.println("<body><h1>ESP32 Web Server</h1>");

  sendOutputState(client, output26, "GPIO 26");
  sendOutputState(client, output27, "GPIO 27");

  client.println("</body></html>");
}

void sendOutputState(WiFiClient client, int pin, const char* label) {
  client.print("<p>");
  client.print(label);
  client.print(" - State ");
  client.print((pin == output26) ? output26State : output27State);
  client.print("</p>");

  if ((pin == output26 && output26State == "off") || (pin == output27 && output27State == "off")) {
    client.print("<p><a href=\"/");
    client.print(pin);
    client.print("/on\"><button class=\"button\">ON</button></a></p>");
  } else {
    client.print("<p><a href=\"/");
    client.print(pin);
    client.print("/off\"><button class=\"button button2\">OFF</button></a></p>");
  }
}
