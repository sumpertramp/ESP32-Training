/*ESP32 BLE 
-ESP32 sadece Wi-Fi ile değil, aynı zamanda Bluetooth ve Bluetooth Low 
Energy (BLE) ile de gelir. Bu yazı, ESP32 ile BLE'nin hızlı bir 
tanıtımını içerir. İlk olarak, BLE'nin ne olduğunu ve ne için 
kullanılabileceğini keşfedeceğiz, ardından Arduino IDE kullanarak ESP32 
ile bazı örnekleri inceleyeceğiz. Basit bir tanıtım için ESP32 BLE 
sunucusu ve bu sunucuyu bulmak için ESP32 BLE tarayıcısı oluşturacağız.

BLE NEDİR?

-Bluetooth Low Energy (BLE), kısaca BLE olarak da bilinir, 
Bluetooth'un güç tasarruflu bir varyantıdır. BLE'nin temel uygulaması, 
düşük bant genişliğiyle küçük miktarda verinin kısa mesafede 
iletilmesidir. Bluetooth'un her zaman açık kalmasının aksine, BLE 
sürekli uyku modunda kalır ve yalnızca bir bağlantı başlatıldığında 
aktifleşir.Bu, çok düşük güç tüketimi sağlar. BLE, kullanım durumuna 
bağlı olarak Bluetooth'a kıyasla yaklaşık 100 kat daha az güç tüketir.

-Ek olarak, BLE sadece noktadan noktaya iletişimi desteklemekle kalmaz, 
aynı zamanda yayın modunu ve örgü ağı (mesh network) desteğini de sağlar.

-Özellikleri nedeniyle, BLE, periyodik olarak çalışan ve bir düğme 
hücresi üzerinde çalışan küçük miktarda veri alışverişi yapması gereken 
uygulamalar için uygundur. Örneğin, BLE, sağlık, fitness, takip, 
işaretçi (beacon), güvenlik ve ev otomasyonu endüstrilerinde büyük 
bir kullanım alanına sahiptir.

BLE SERVER AND CLIENT

-ESP32, istemci veya sunucu olarak hareket edebilir.

Sunucu, varlığını reklam eder, böylece diğer cihazlar tarafından 
bulunabilir ve istemcinin okuyabileceği verileri içerir. İstemci,
yakındaki cihazları tarama yapar ve aradığı sunucuyu bulduğunda bir 
bağlantı kurar ve gelen verileri dinler. Bu, noktadan noktaya iletişim 
olarak adlandırılır.

Daha önce belirtildiği gibi, BLE ayrıca yayın modunu ve örgü ağını da 
destekler. Yayın modu: Sunucu, bağlı birçok istemciye aynı anda veri 
iletebilir.
Örgü ağı: Tüm cihazlar birbirine bağlıdır ve birçok cihaz arasında 
bağlantı kurulmasını sağlar.
BLE ile yayın modunu ve örgü ağını uygulamak mümkün olsa da, 
bu özellikler daha yeni geliştirilmiş olup, şu anda ESP32 için özel 
olarak çok sayıda örnek veya uygulama bulunmayabilir.

GATT

-GATT (Generic Attribute Profile), Generic Attributes'ın kısaltmasıdır 
ve bağlı BLE cihazlara sunulan hiyerarşik bir veri yapısını tanımlar. 
Bu, GATT'ın iki BLE cihazının standart mesajlar gönderip almasının 
yolunu tanımladığı anlamına gelir. Bu hiyerarşiyi anlamak önemlidir, 
çünkü BLE'yi nasıl kullanacağınızı ve uygulamalarınızı nasıl 
yazacağınızı daha kolay anlamanıza yardımcı olacaktır.

BLE SERVICE

-Hiyerarşinin en üst seviyesinde bir profil bulunur ve bu profil bir 
veya daha fazla servisten oluşur. Genellikle, bir BLE cihazı birden 
fazla servis içerir.Her servis en az bir karakteristik içerir veya diğer 
servislere referans olabilir. Bir servis sadece sensör okumaları gibi 
bilgilerin bir koleksiyonudur. Bluetooth Special Interest Group (SIG) 
tarafından belirlenen çeşitli veri türleri için önceden tanımlanmış 
servisler vardır, örneğin Pil Seviyesi, Kan Basıncı, Kalp Atış Hızı, 
Ağırlık Ölçeği vb. Diğer tanımlanmış servisleri buradan kontrol 
edebilirsiniz.

BLE CHARACTERISTIC

-Karakteristik her zaman bir servise aittir ve verinin (değer) gerçekte 
nerede bulunduğunu içerir. Karakteristik her zaman iki özelliğe 
sahiptir: karakteristik beyanı (veri hakkında meta veriler sağlar) ve 
karakteristik değeri.Ek olarak, karakteristik değer, karakteristik 
beyanında bulunan meta verileri daha da genişleten tanımlayıcılarla 
takip edilebilir.Özellikler, karakteristik değerin nasıl etkileşime 
girebileceğini tanımlar. Temel olarak, karakteristikle kullanılabilen 
işlemleri ve prosedürleri içerir:

-Yayın (Broadcast)
-Oku (Read)
-Yanıtsız Yazma (Write without response)
-Yazma (Write)
-Bildirim (Notify)
-Gösterim (Indicate)
-Kimlik Doğrulamalı İmzalı Yazma (Authenticated Signed Writes)
-Genişletilmiş Özellikler (Extended Properties)

UUID

Her servis, karakteristik ve tanımlayıcı, bir UUID'ye (Evrensel Benzersiz
Tanımlayıcı) sahiptir. Bir UUID, benzersiz bir 128 bit (16 bayt) 
numaradır. Örneğin:
   -55072829-bc9e-4c53-938a-74a6d4c78776

SIG (Bluetooth Special Interest Group) tarafından belirtilen tüm türler, 
servisler ve profiller için kısaltılmış UUID'ler vardır. Ancak, 
uygulamanız kendi UUID'ye ihtiyaç duyuyorsa, bu UUID oluşturma web 
sitesini kullanarak bunu oluşturabilirsiniz. Özet olarak, UUID bilgilerin
benzersiz bir şekilde tanımlanması için kullanılır. Örneğin, Bluetooth 
cihazı tarafından sağlanan belirli bir servisi tanımlayabilir.

-*/

/*BLE SUNUCUSU OLUŞTURMAK İÇİN KOD ÖRNEĞİ*/
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// UUID oluşturmak için aşağıdaki linke bakın:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

void setup() {
  Serial.begin(115200);
  Serial.println("BLE çalışmasına başlanıyor!");

  BLEDevice::init("Uzun isim şimdi çalışıyor");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setValue("Merhaba Dünya diyor Neil");
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // bu, geriye dönük uyumluluk için hala çalışıyor
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // iPhone bağlantı sorunuyla ilgili yardımcı olan işlevler
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Karakteristik tanımlandı! Şimdi telefonunuzda okuyabilirsiniz!");
}

void loop() {
  // Ana kodunuzu buraya yazarak tekrarlı olarak çalıştırın:
  delay(100);
}
