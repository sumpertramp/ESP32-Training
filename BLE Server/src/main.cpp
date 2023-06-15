/* BLE SERVER AND CLIENT

İki ESP32 kartı arasında BLE (Bluetooth Low Energy) bağlantısı nasıl 
kurulacağını öğrenelim. Bir ESP32 sunucu olarak kullanılacak ve diğer 
ESP32 ise istemci olarak kullanılacak. BLE sunucusu, istemcinin okuyabileceği 
sensör verilerini içeren özellikleri reklam edecek. ESP32 BLE istemcisi bu 
özelliklerin değerlerini (sıcaklık ve nem) okuyacak ve bunları bir OLED 
ekranda gösterecektir.

-
Bluetooth Low Energy (BLE) ile, iki tür cihaz bulunur: sunucu ve istemci. 

-Sunucu varlığını duyurarak diğer cihazlar tarafından bulunabilir ve 
istemcinin okuyabileceği verileri içerir. İstemci çevredeki cihazları 
tarar ve aradığı sunucuyu bulduğunda bir bağlantı kurar ve gelen verileri 
dinler. Bu noktadan noktaya iletişim olarak adlandırılır.ESP32, istemci 
veya sunucu olarak hareket edebilir.

GATT

Generic Attributes olarak adlandırılır ve bağlı BLE cihazlara sunulan bir 
hiyerarşik veri yapısını tanımlar. Bu, GATT'ın iki BLE cihazının standart 
mesajları nasıl gönderip alacaklarını belirlediği anlamına gelir. Bu 
hiyerarşiyi anlamak önemlidir çünkü ESP32 ile BLE kullanımını anlamak için 
kolaylık sağlar.

Profile: Belirli bir kullanım durumu için standart bir hizmet koleksiyonu.
Service: Sensör okumaları, pil seviyesi, kalp atış hızı vb. gibi ilgili 
bilgilerin bir araya getirildiği bir koleksiyon.
Characteristic: Hiyerarşideki gerçek verilerin saklandığı yerdir (değer).
Descriptor: Veri hakkında meta bilgiler içerir. 
Properties: Karakteristik değerin nasıl etkileşime girebileceğini açıklar.
Örneğin: okuma, yazma, bildirim, yayın, gösterim vb.


Örneğimizde, iki özelliğe sahip bir hizmet oluşturacağız. Biri sıcaklık için, 
diğeri nem için. Gerçek sıcaklık ve nem okumaları, özelliklerinin altındaki 
değerlerde saklanır. Her bir özellik bildirim özelliğine sahiptir, böylece
değerler değiştiğinde istemciye bildirim gönderir.

Bu bölümde, BLE Sunucusunu kuracağız. Sunucu, sıcaklık ve nem için iki 
özelliği içeren bir hizmeti duyuracak. Bu özellikler, istemciye yeni 
değerleri bildirmek için Notify özelliğine sahiptir.

ESP32 BLE sunucusu, BME280 sensöründen alınan sıcaklık ve nem özelliklerini 
duyuracaktır. İlgili kodlara eklemek şartıyla başka bir sensör de 
kullanabilirsiniz.

BME280 sensör modülü ile I2C iletişimini kullanacağız. Bunun için sensörü 
varsayılan ESP32 SCL (GPIO 22) ve SDA (GPIO 21) pinlerine bağlayın. 

*/

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

//Varsayılan sıcaklık Celsius cinsinden
//Sıcaklığı Fahrenheit cinsinden almak için aşağıdaki satırı yorum satırı yapın
#define temperatureCelsius

//BLE sunucusu adı
#define bleServerName "BME280_ESP32"

Adafruit_BME280 bme; // I2C

float temp;
float tempF;
float hum;

// Zamanlayıcı değişkenleri
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

bool deviceConnected = false;

// UUID'ler için aşağıdaki linke bakın:
// https://www.uuidgenerator.net/
#define SERVICE_UUID "91bad492-b950-4226-aa2b-4ede9fa42f59"

// Sıcaklık Karakteristiği ve Tanımlayıcı
#ifdef temperatureCelsius
  BLECharacteristic bmeTemperatureCelsiusCharacteristics("cba1d466-344c-4be3-ab3f-189f80dd7518", BLECharacteristic::PROPERTY_NOTIFY);
  BLEDescriptor bmeTemperatureCelsiusDescriptor(BLEUUID((uint16_t)0x2902));
#else
  BLECharacteristic bmeTemperatureFahrenheitCharacteristics("f78ebbff-c8b7-4107-93de-889a6a06d408", BLECharacteristic::PROPERTY_NOTIFY);
  BLEDescriptor bmeTemperatureFahrenheitDescriptor(BLEUUID((uint16_t)0x2902));
#endif

// Nem Karakteristiği ve Tanımlayıcı
BLECharacteristic bmeHumidityCharacteristics("ca73b3ba-39f6-4ab3-91ae-186dc9577d99", BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor bmeHumidityDescriptor(BLEUUID((uint16_t)0x2903));

//Bağlantı kurulduğunda ve kesildiğinde geri çağırma işlevleri
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};

void initBME(){
  if (!bme.begin(0x76)) {
    Serial.println("Geçerli bir BME280 sensörü bulunamadı, bağlantıları kontrol edin!");
    while (1);
  }
}

void setup() {
  // Seri iletişimi başlat
  Serial.begin(115200);

  // BME Sensörünü başlat
  initBME();

  // BLE Cihazını oluştur
  BLEDevice::init(bleServerName);

  // BLE Sunucusunu oluştur
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // BLE Servisini oluştur
  BLEService *bmeService = pServer->createService(SERVICE_UUID);

  // BLE Karakteristiklerini ve BLE Tanımlayıcılarını oluştur
  // Sıcaklık
  #ifdef temperatureCelsius
    bmeService->addCharacteristic(&bmeTemperatureCelsiusCharacteristics);
    bmeTemperatureCelsiusDescriptor.setValue("BME sıcaklık Celsius");
    bmeTemperatureCelsiusCharacteristics.addDescriptor(&bmeTemperatureCelsiusDescriptor);
  #else
    bmeService->addCharacteristic(&bmeTemperatureFahrenheitCharacteristics);
    bmeTemperatureFahrenheitDescriptor.setValue("BME sıcaklık Fahrenheit");
    bmeTemperatureFahrenheitCharacteristics.addDescriptor(&bmeTemperatureFahrenheitDescriptor);
  #endif  

  // Nem
  bmeService->addCharacteristic(&bmeHumidityCharacteristics);
  bmeHumidityDescriptor.setValue("BME nem");
  bmeHumidityCharacteristics.addDescriptor(new BLE2902());
  
  // Servisi başlat
  bmeService->start();

  // İlanı başlat
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pServer->getAdvertising()->start();
  Serial.println("Bağlantı bekleniyor...");
}

void loop() {
  if (deviceConnected) {
    if ((millis() - lastTime) > timerDelay) {
      // Sıcaklığı Celsius olarak oku (varsayılan)
      temp = bme.readTemperature();
      // Fahrenheit olarak oku
      tempF = 1.8*temp +32;
      // Nem oku
      hum = bme.readHumidity();
  
      // BME sensöründen sıcaklık okumasını bildir
      #ifdef temperatureCelsius
        static char temperatureCTemp[6];
        dtostrf(temp, 6, 2, temperatureCTemp);
        // Sıcaklık karakteristiği değerini ayarla ve bağlı istemciye bildir
        bmeTemperatureCelsiusCharacteristics.setValue(temperatureCTemp);
        bmeTemperatureCelsiusCharacteristics.notify();
        Serial.print("Sıcaklık Celsius: ");
        Serial.print(temp);
        Serial.print(" ºC");
      #else
        static char temperatureFTemp[6];
        dtostrf(tempF, 6, 2, temperatureFTemp);
        // Sıcaklık karakteristiği değerini ayarla ve bağlı istemciye bildir
        bmeTemperatureFahrenheitCharacteristics.setValue(temperatureFTemp);
        bmeTemperatureFahrenheitCharacteristics.notify();
        Serial.print("Sıcaklık Fahrenheit: ");
        Serial.print(tempF);
        Serial.print(" ºF");
      #endif
      
      // BME'den nem okumasını bildir
      static char humidityTemp[6];
      dtostrf(hum, 6, 2, humidityTemp);
      // Nem karakteristiği değerini ayarla ve bağlı istemciye bildir
      bmeHumidityCharacteristics.setValue(humidityTemp);
      bmeHumidityCharacteristics.notify();   
      Serial.print(" - Nem: ");
      Serial.print(hum);
      Serial.println(" %");
      
      lastTime = millis();
    }
  }
}

