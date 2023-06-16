/* ESP32 Bluetooth

-Bluetooth Classic kullanmak Bluetooth Low Energy'ye göre çok daha 
basittir. Eğer daha önce HC-06 gibi bir Bluetooth modülüyle Arduino 
programladıysanız, bu oldukça benzerdir. Standart seri protokolünü ve 
işlevleri kullanır.

-Akıllı telefonunuzda bir Bluetooth Terminal uygulaması kurulu olması 
gerekmektedir.Play Store'da bulunan "Serial Bluetooth Terminal" adlı 
Android uygulamasını kullanmanızı öneririz.
*/

#include <Arduino.h>
// Kütüphaneleri yükle
#include "BluetoothSerial.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// Bluetooth yapılandırmalarının etkinleştirilip etkinleştirilmediğini kontrol et
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth etkin değil! Lütfen make menuconfig komutunu çalıştırın ve etkinleştirin.
#endif

// Bluetooth Seri nesnesi
BluetoothSerial SerialBT;

// LED'in bağlı olduğu GPIO pini
const int ledPin = 25;

// DS18B20'nin bağlı olduğu GPIO pini
const int oneWireBus = 32;
// OneWire cihazlarıyla iletişim kurmak için bir OneWire örneği oluştur
OneWire oneWire(oneWireBus);
// OneWire referansını Dallas Temperature sensörüne aktar
DallasTemperature sensors(&oneWire);

// Alınan ve gönderilen mesajları işle
String message = "";
char incomingChar;
String temperatureString = "";

// Zamanlayıcı: yardımcı değişkenler
unsigned long previousMillis = 0; // Sıcaklık yayınlanma zamanını depolar
const long interval = 10000; // Sensör okumalarının yayınlanacağı aralık

void setup() {
pinMode(ledPin, OUTPUT);
Serial.begin(115200);
// Bluetooth cihaz adı
SerialBT.begin("ESP32");
Serial.println("Cihaz başladı, şimdi onu Bluetooth ile eşleyebilirsiniz!");
}

void loop() {
unsigned long currentMillis = millis();
// Sıcaklık okumalarını gönder
if (currentMillis - previousMillis >= interval){
previousMillis = currentMillis;
sensors.requestTemperatures();
temperatureString = String(sensors.getTempCByIndex(0)) + "C " + String(sensors.getTempFByIndex(0)) + "F";
SerialBT.println(temperatureString);
}
// Alınan mesajları oku (LED kontrol komutu)
if (SerialBT.available()){
char incomingChar = SerialBT.read();
if (incomingChar != '\n'){
message += String(incomingChar);
}
else{
message = "";
}
Serial.write(incomingChar);
}
// Alınan mesajı kontrol et ve çıkışı buna göre kontrol et
if (message =="led_on"){
digitalWrite(ledPin, HIGH);
}
else if (message =="led_off"){
digitalWrite(ledPin, LOW);
}
delay(20);
}