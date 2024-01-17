#include <WiFi.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>

const char *ssid_AP = "ESP32-Access-Point";
const char *password_AP = "123456789";

char* username;
char* password;

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.print("Setting up AP (Access Point)...");

  // Configuring the Access Point
  WiFi.softAP(ssid_AP, password_AP);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Serve static files from SPIFFS
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  // Set up server routes
  server.on("/submit", HTTP_POST, handlePost);

  // Start the server
  server.begin();

  // Mount SPIFFS file system
  if (SPIFFS.begin()) {
    Serial.println("SPIFFS mounted successfully!");
  } else {
    Serial.println("Failed to mount SPIFFS!");
  }
}

void loop() {
  // Nothing to be done here
}

void handlePost(AsyncWebServerRequest *request) {
  Serial.println("New POST request received");

  // Get username and password from the request
  String username = request->arg("username");
  String password = request->arg("password");

  // Saving username and password to a file
  if (saveNetworkCredentialsToSPIFFS(username, password)) {
    request->send(200, "text/plain", "Credentials saved successfully!");

    // Reading and sending the content of the file
    String fileContent = readCredentialsFromSPIFFS();
    request->send(200, "text/plain", "File Content:\n" + fileContent);
  } else {
    request->send(500, "text/plain", "Failed to save credentials!");
  }
}

bool saveNetworkCredentialsToSPIFFS(String username, String password) {
  // Open the file in write mode
  File file = SPIFFS.open("/credentials.txt", "w");
  if (!file) {
    Serial.println("Failed to open file for writing!");
    return false;
  }

  // Write username and password to the file
  file.println("Username: " + username);
  file.println("Password: " + password);

  // Close the file
  file.close();
  Serial.println("Credentials saved to /credentials.txt");
  return true;  
}

String readCredentialsFromSPIFFS() {
  String fileContent = "";

  // Open the file in read mode
  File file = SPIFFS.open("/credentials.txt", "r");
  if (file) {
    // Read the file content
    while (file.available()) {
      fileContent += char(file.read());
    }
    // Close the file
    file.close();
  }
  else {
    Serial.println("Failed to open file for reading!");
  }
  Serial.println(fileContent);
  return fileContent;
}
