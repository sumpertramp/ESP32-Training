#include <WiFi.h>
#include <WiFiClient.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>

TaskHandle_t Task1;
TaskHandle_t Task2;

#define TXD 17
#define RXD 16

String data_UART;

const int WiFiPin = 26;
const int startPin = 27;

const char* ssid_AP = "AGV-001";
const char* password_AP = "ermas123";

//Set web server pot number to 80
AsyncWebServer server(80);

// milis() timer
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(115200,SERIAL_8N1, RXD, TXD);

  pinMode(WiFiPin, OUTPUT);
  pinMode(startPin, OUTPUT);
  digitalWrite(WiFiPin, LOW);

  // Task1
  xTaskCreatePinnedToCore(Task1code, "Task1", 10000, NULL, 1, &Task1, 0);
  delay(500);

  // Task2
  xTaskCreatePinnedToCore(Task2code, "Task2", 10000, NULL, 1, &Task2, 1);
  delay(500);

  if(SPIFFS.begin()) {
    Serial.println("SPIFFS mounted successfully!");

    // Check if username and password are present in SPIFFS
    String storedUsername = readStoredUsernameFromSPIFFS();
    String storedPassword = readStoredPasswordFromSPIFFS();
    String storedIP = readStoredIPFromSPIFFS();
    String storedPort = readStoredPortFromSPIFFS();

    Serial.println(storedUsername);
    Serial.println(storedPassword);
    Serial.println(storedIP);
    Serial.println(storedPort);

    if (storedUsername.isEmpty() || storedPassword.isEmpty() || storedIP.isEmpty() || storedPort.isEmpty()) {
      // No username or password found in SPIFFS, set up Access Point
      Serial.println("No stored credentials found. Starting in Access Point mode...");
      startAccessPointMode();
    } else {
      File file = SPIFFS.open("/credentials.txt", "r");
      Serial.println("Stored credentials found. Connecting to WiFi...");
      connectToServer(storedUsername, storedPassword);

      // Close the access point mode if currently active
      if (WiFi.getMode() == WIFI_AP) {
         
        Serial.println("Closing access point mode..");
        WiFi.softAPdisconnect(true);
      }
    }
  } else {
    Serial.println("Failed to mount SPIFFS!");
  }
}

void Task1code(void * pvParameters ){
  for (;;) {
    String serverIP = readStoredIPFromSPIFFS();
    String serverPort = readStoredPortFromSPIFFS();
    delay(10);

    WiFiClient client;
    client.connect(serverIP.c_str(), serverPort.toInt());
    
    client.println(data_UART);
    delay(100);
  }
}

void Task2code (void * pvParameters) {
  for (;;) {
    delay(10);
    data_UART = Serial2.readStringUntil('\n');
    //Serial.println(data_UART); 
    delay(10);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}

String readStoredUsernameFromSPIFFS() {
  //Serial.println(readStoredValueFromSPIFFS("Username:"));
  return readStoredValueFromSPIFFS("Username:");
}

String readStoredPasswordFromSPIFFS() {
  //Serial.println(readStoredValueFromSPIFFS("Password:"));
  return readStoredValueFromSPIFFS("Password:");
}

String readStoredIPFromSPIFFS() {
  //Serial.println(readStoredValueFromSPIFFS("IP:"));
  return readStoredValueFromSPIFFS("IP:");
}

String readStoredPortFromSPIFFS() {
  //Serial.println(readStoredValueFromSPIFFS("Port:"));
  return readStoredValueFromSPIFFS("Port:");
}

String readStoredValueFromSPIFFS(const char *prefix) {
  String storedValue = "";
  File file = SPIFFS.open("/credentials.txt", "r");
  if (file) {
    while (file.available()) {
      String line = file.readStringUntil('\n');
      if (line.startsWith(prefix)) {
        storedValue = line.substring(strlen(prefix));
      }
    }
    file.close();
  }
  return storedValue;
}

void startAccessPointMode() {
  Serial.println("Starting in Access Point mode...");
  
  // Switch to Access Point mode
  WiFi.softAP(ssid_AP, password_AP);
  IPAddress IP = WiFi.softAPIP();
  Serial.println("AP IP address: ");
  Serial.println(IP);

  // Notification to client that access point connestion is successful.
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "AP");
  });

  // Serve static files from SPIFFS
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  // Set up server routes
  server.on("/submit", HTTP_POST, handlePost);

  // Start the server
  server.begin();
}

void connectToServer(String username, String password) {
  Serial.println("Connecting to WiFi...");

  // Use the stored username and password to connect to WiFi
  Serial.print("Username: ");
  Serial.println(username.c_str());

  Serial.print("Password: ");
  Serial.println(password.c_str());
  WiFi.begin(username.c_str(), password.c_str());
  delay(10);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    digitalWrite(WiFiPin, HIGH);
    delay(500);
    digitalWrite(WiFiPin, LOW);
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to WiFi!");
    //Connect to the server
    //Serial.print("IP: ");
    //Serial.println(ip.c_str());
    //Serial.println("Port: " + port.toInt());
  } else {
    Serial.println("\nFailed to connect to WiFi. Please check your credentials.");
    startAccessPointMode();
    clearCredentialsInSPIFFS(); 
  }
}

void clearCredentialsInSPIFFS() {
  // Open the file in write mode to clear its content
  File file = SPIFFS.open("/credentials.txt", "w");
  if (!file) {
    Serial.println("Failed to open file for writing!");
    return;
  }
  // Close the file
  file.close();
  Serial.println("Credentials in /credentials.txt cleared!");
}

void handlePost(AsyncWebServerRequest *request) {
  Serial.println("New POST request received");

  // Get username and password from the request
  String username = request->arg("username");
  String password = request->arg("password");
  String ip = request->arg("ip");
  String port = request->arg("port");

  // Saving username and password to a file
  if (saveNetworkCredentialsToSPIFFS(username, password, ip, port)) {
    request->send(200, "text/plain", "Credentials saved successfully!");

    // Reading and sending the content of the file
    String fileContent = readCredentialsFromSPIFFS();
    request->send(200, "text/plain", "File Content:\n" + fileContent);

    // Connect to WiFi with the new credentials
    connectToServer(username, password);
  } else {
    request->send(500, "text/plain", "Failed to save credentials!");
  }  
}

bool saveNetworkCredentialsToSPIFFS(String username, String password, String ip, String port) {
  // Open the file in write mode
  File file = SPIFFS.open("/credentials.txt", "w");
  if (!file) {
    Serial.println("Failed to open file for writing!");
    return false;
  }

  // Write username and password to the file
  file.println("Username:" + username);
  file.println("Password:" + password);
  file.println("IP:" + ip);
  file.println("Port:" + port);

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

void sendDataToServerPeriodically() {
  //Define the server IP address and port
  String serverIP = readStoredIPFromSPIFFS();
  String serverPort = readStoredPortFromSPIFFS();

  //Check if server IP and port are available
  if (serverIP.isEmpty() || serverPort.isEmpty()) {
    Serial.println("Server IP and port not available. Cannot send data.");
    return;
  }
  
  //Atempt to connect to the server
  WiFiClient client;
  if (!client.connect(serverIP.c_str(), serverPort.toInt())) {
    Serial.println("Failed to connect to server. Cannot send data.");
    return;
  }

  //Successfully connected to server, now send data
  Serial.println("Sending data to server.");
  client.println(data_UART);
  //Serial.println(data_UART);
  //client.stop();
  Serial.println("Data sent to server.");
}







