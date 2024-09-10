 /*
  Sümeyye Ünal
  Embedded Software Engineer

    This program retrieves and displays the default SPI pin configuration (MOSI, MISO, SCK, SS)
  for the ESP32 board using the Serial Monitor. It initializes the serial communication
  at a baud rate of 115200, and then prints the pin numbers associated with the
  standard SPI functions. This can help verify the correct SPI pin setup for your
  ESP32 board before interfacing with SPI devices.
*/

//Find the default SPI pins for your board
//Make sure you have the right board selected in Tools > Boards
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("MOSI: ");
  Serial.println(MOSI);
  Serial.print("MISO: ");
  Serial.println(MISO);
  Serial.print("SCK: ");
  Serial.println(SCK);
  Serial.print("SS: ");
  Serial.println(SS);  
}

void loop() {
  // put your main code here, to run repeatedly:
}
