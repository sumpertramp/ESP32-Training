/* ESP32 BLE CLIENT

Bu bölümde, ESP32 BLE sunucusu ile bağlantı kuracak olan ESP32 BLE 
istemcisini oluşturacağız ve okumaları bir OLED ekranında görüntüleyeceğiz.

*/

#include <Arduino.h>

#include "BLEDevice.h"
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

// Varsayılan sıcaklık Celsius olarak ayarlanmıştır
// Fahrenheit olarak sıcaklık için bu satırın önündeki yorum işaretini kaldırın
#define temperatureCelsius

// BLE Sunucusu adı (sunucu kodunu çalıştıran diğer ESP32 adı)
#define bleServerName "BME280_ESP32"

/* Okumak istediğimiz hizmetin ve karakteristiğin UUID'leri */
// BLE Hizmeti
static BLEUUID bmeServiceUUID("91bad492-b950-4226-aa2b-4ede9fa42f59");

// BLE Karakteristikleri
#ifdef temperatureCelsius
  // Sıcaklık Celsius Karakteristiği
  static BLEUUID temperatureCharacteristicUUID("cba1d466-344c-4be3-ab3f-189f80dd7518");
#else
  // Sıcaklık Fahrenheit Karakteristiği
  static BLEUUID temperatureCharacteristicUUID("f78ebbff-c8b7-4107-93de-889a6a06d408");
#endif

// Nem Karakteristiği
static BLEUUID humidityCharacteristicUUID("ca73b3ba-39f6-4ab3-91ae-186dc9577d99");

// Bağlantı kurulup kurulmadığını ve bağlantının aktif olup olmadığını belirten bayraklar
static boolean doConnect = false;
static boolean connected = false;

// Periferik cihazın adresi. Adres tarama sırasında bulunacak...
static BLEAddress *pServerAddress;

// Okumak istediğimiz karakteristikler
static BLERemoteCharacteristic* temperatureCharacteristic;
static BLERemoteCharacteristic* humidityCharacteristic;

// Bildirimleri etkinleştirme
const uint8_t notificationOn[] = {0x1, 0x0};
const uint8_t notificationOff[] = {0x0, 0x0};

#define SCREEN_WIDTH 128 // OLED ekran genişliği, piksel cinsinden
#define SCREEN_HEIGHT 64 // OLED ekran yüksekliği, piksel cinsinden

// I2C'ye bağlı bir SSD1306 ekranı için bildirim
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Sıcaklık ve nem değerlerini depolamak için değişkenler
char* temperatureChar;
char* humidityChar;

// Yeni sıcaklık ve nem okumalarının mevcut olup olmadığını kontrol etmek için bayraklar
boolean newTemperature = false;
boolean newHumidity = false;

// İsimlendirilmiş BLE Sunucusuna bağlanma işlemi
bool connectToServer(BLEAddress pAddress) {
   BLEClient* pClient = BLEDevice::createClient();
 
  // Uzak BLE Sunucusuna bağlan
  pClient->connect(pAddress);
  Serial.println(" - Sunucuya bağlandı");
 
  // Uzaktaki BLE sunucusundaki hizmete bir referans al
  BLERemoteService* pRemoteService = pClient->getService(bmeServiceUUID);
  if (pRemoteService == nullptr) {
    Serial.print("Servis UUID'sini bulma başarısız oldu: ");
    Serial.println(bmeServiceUUID.toString().c_str());
    return (false);
  }
 
  // Uzaktaki BLE sunucusundaki karakteristiklere bir referans al
  temperatureCharacteristic = pRemoteService->getCharacteristic(temperatureCharacteristicUUID);
  humidityCharacteristic = pRemoteService->getCharacteristic(humidityCharacteristicUUID);

  if (temperatureCharacteristic == nullptr || humidityCharacteristic == nullptr) {
    Serial.print("Karakteristik UUID'sini bulma başarısız oldu");
    return false;
  }
  Serial.println(" - Karakteristiklerimizi bulduk");
 
  // Karakteristikler için geri çağırma işlevlerini atama
  temperatureCharacteristic->registerForNotify(temperatureNotifyCallback);
  humidityCharacteristic->registerForNotify(humidityNotifyCallback);
  return true;
}

// Bir cihazın reklamı alındığında çağrılan geri çağırma işlevi
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    if (advertisedDevice.getName() == bleServerName) { // Reklam verenin adıyla eşleşiyor mu kontrol et
      advertisedDevice.getScan()->stop(); // Tarama durdurulabilir, aradığımızı bulduk
      pServerAddress = new BLEAddress(advertisedDevice.getAddress()); // Reklam verenin adresi ihtiyacımız olan adres
      doConnect = true; // Bağlanmaya hazır olduğumuzu belirten göstergeyi ayarla
      Serial.println("Cihaz bulundu. Bağlanılıyor!");
    }
  }
};
 
// BLE Sunucusu yeni bir sıcaklık okuması bildirimi gönderdiğinde çağrılan geri çağırma işlevi
static void temperatureNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, 
                                        uint8_t* pData, size_t length, bool isNotify) {
  // Sıcaklık değerini depola
  temperatureChar = (char*)pData;
  newTemperature = true;
}

// BLE Sunucusu yeni bir nem okuması bildirimi gönderdiğinde çağrılan geri çağırma işlevi
static void humidityNotifyCallback(BLERemoteCharacteristic* pBLERemoteCharacteristic, 
                                    uint8_t* pData, size_t length, bool isNotify) {
  // Nem değerini depola
  humidityChar = (char*)pData;
  newHumidity = true;
  Serial.print(newHumidity);
}

// En son sensör okumalarını OLED ekranda yazdıran işlev
void printReadings(){
  
  display.clearDisplay();  
  // Sıcaklığı ekranda göster
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("Sıcaklık: ");
  display.setTextSize(2);
  display.setCursor(0,10);
  display.print(temperatureChar);
  display.setTextSize(1);
  display.cp437(true);
  display.write(167);
  display.setTextSize(2);
  Serial.print("Sıcaklık:");
  Serial.print(temperatureChar);
  #ifdef temperatureCelsius
    // Celsius Sıcaklık
    display.print("C");
    Serial.print("C");
  #else
    // Fahrenheit Sıcaklık
    display.print("F");
    Serial.print("F");
  #endif

  // Nem değerini ekranda göster
  display.setTextSize(1);
  display.setCursor(0, 35);
  display.print("Nem: ");
  display.setTextSize(2);
  display.setCursor(0, 45);
  display.print(humidityChar);
  display.print("%");
  display.display();
  Serial.print(" Nem:");
  Serial.print(humidityChar); 
  Serial.println("%");
}

void setup() {
  // OLED ekran kurulumu
  // SSD1306_SWITCHCAPVCC = 3.3V'dan ekran voltajı oluştur
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // 128x32 için Adres 0x3C
    Serial.println(F("SSD1306 ayrıştırması başarısız"));
    for(;;); // İlerlemeyin, sonsuza kadar döngüde kalın
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE,0);
  display.setCursor(0,25);
  display.print("BLE İstemcisi");
  display.display();
  
  // Seri iletişimi başlat
  Serial.begin(115200);
  Serial.println("Arduino BLE İstemci uygulaması başlatılıyor...");

  // BLE cihazını başlat
  BLEDevice::init("");
 
  // Bir Tarayıcı al ve yeni bir cihaz algılandığında bilgilendirilmek için kullanmak istediğimiz geri çağırma işlevini ayarla.
  // Aktif tarama yapılmasını belirtin ve taramayı 30 saniye çalıştırın.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  pBLEScan->start(30);
}

void loop() {
  // "doConnect" bayrağı true ise tarandı ve istediğimiz BLE Sunucusunu bulduk.
  // Şimdi ona bağlanıyoruz. Bağlandığımızda connected bayrağını true olarak ayarlıyoruz.
  if (doConnect == true) {
    if (connectToServer(*pServerAddress)) {
      Serial.println("Artık BLE Sunucusuna bağlandık.");
      // Her Karakteristik için Bildirim özelliğini etkinleştir
      temperatureCharacteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notificationOn, 2, true);
      humidityCharacteristic->getDescriptor(BLEUUID((uint16_t)0x2902))->writeValue((uint8_t*)notificationOn, 2, true);
      connected = true;
    } else {
      Serial.println("Sunucuya bağlanamadık; Yakındaki BLE sunucusunu taramak için cihazınızı yeniden başlatın.");
    }
    doConnect = false;
  }
  // Yeni sıcaklık okumaları mevcutsa, OLED ekrana yazdır
  if (newTemperature && newHumidity){
    newTemperature = false;
    newHumidity = false;
    printReadings();
  }
  delay(1000); // Döngüler arasında bir saniye gecikme.
}
