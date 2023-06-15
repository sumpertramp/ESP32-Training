/*ESP32 ADC
ESP32 ile analog girişleri nasıl okuyacağınızı içeren kod dizisidir. 
Analog okuma, potansiyometre gibi değişken dirençlerden veya analog 
sensörlerden değer okumak için faydalıdır.
-ESP32 ile analog girişleri okumak, okumak istediğiniz GPIO'yu argüman 
olarak kabul eden analogRead(GPIO) işlevini kullanmak kadar kolaydır.
-ESP32 ile analog bir değer okumak, 0 V ile 3.3 V arasında değişen 
gerilim seviyelerini ölçebilmeniz anlamına gelir.
-Ölçülen gerilim daha sonra 0 ile 4095 arasında bir değere atanır, 
burada 0 V'ye 0 ve 3.3 V'ye 4095 karşılık gelir. 0 V ile 3.3 V 
arasındaki herhangi bir gerilim, aradaki uygun değeri alacaktır.
-deal olarak, ESP32 ADC pinlerini kullandığınızda lineer bir davranış 
beklersiniz. Ancak, bu gerçekleşmez. Bu davranış, ESP32'nizin 3.3 V'yi 
3.2 V'den ayırt edememesi anlamına gelir. Her iki gerilim için de aynı 
değeri alırsınız: 4095. Aynı durum çok düşük gerilim değerleri için de 
geçerlidir: 0 V ve 0.1 V için aynı değeri alırsınız: 0. ESP32 ADC 
pinlerini kullanırken bunu göz önünde bulundurmanız önemlidir. 
-ESP32, 18 farklı kanalda ölçüm yapmayı destekler. Ancak, DEVKIT V1 DOIT 
kartında (30 GPIO'lu versiyon) sadece 15 kanal kullanılabilir durumdadır.
-Bu analog giriş pinleri 12 bit çözünürlüğe sahiptir. Bu, bir analog 
girişi okuduğunuzda aralığının 0 ila 4095 arasında değişebileceği 
anlamına gelir.*/

#include <Arduino.h>

const int potPin = 34;
/*const int pin = 16;
const int resolution = 12;
const int width = 12;
const int cycles = 8;
const int samples = 1;
const int attenuation0 = 1;
const int attenuation1 = 1;*/

int potValue = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  /*analogReadResolution(resolution); /*Örnekleme bitlerini ve çözünürlüğü
  ayarlar. Değer olarak 9 (0 - 511) ile 12 bit (0 - 4095) arasında 
  bir değer olabilir. Varsayılan olarak 12 bit çözünürlük kullanılır.*/

  /*analogSetWidth(width); /*Örneklem bitlerini ve çözünürlüğü set eder. 
  Değer, 9 (0 - 511) ile 12 bit (0 - 4095) arasında olabilir.
  Varsayılan olarak 12 bit çözünürlük kullanılır.*/
  
  /*analogSetCycles(cycles); /*Örnekleme başına döngü sayısını ayarlar. 
  Varsayılan değer 8'dir. Aralık: 1 ile 255 arasında olabilir.*/
  
  /*analogSetSamples(samples); /*Örneklem aralığındaki örnek sayısını 
  ayarlar. Varsayılan olarak 1 örnek kullanılır. Bu, hassasiyeti 
  artırma etkisine sahiptir.*/

  /*analogSetClockDiv(attenuation0);/*ADC saatine bölücüyü ayarlar. 
  Varsayılan değer 1'dir. Aralık: 1 ile 255 arasında olabilir.*/

  /*analogSetAttenuation("ADC_0db"); /*Tüm ADC pinleri için giriş 
  zayıflatmasını ayarlar. Varsayılan değer ADC_11db'dir. Kabul edilen 
  değerler şunlardır:
    -ADC_0db: Zayıflatma yapılmaz. ADC yaklaşık olarak 800 mV'ye kadar 
  ölçüm yapabilir (1V giriş = ADC okuması 1088).
    -ADC_2_5db: ADC'nin giriş gerilimi zayıflatılır, ölçüm aralığı 
  yaklaşık olarak 1100 mV'ye kadar genişletilir (1V giriş = ADC okuması 
  3722).
    -ADC_6db: ADC'nin giriş gerilimi zayıflatılır, ölçüm aralığı yaklaşık
  olarak 1350 mV'ye kadar genişletilir (1V giriş = ADC okuması 3033).
    -ADC_11db: ADC'nin giriş gerilimi zayıflatılır, ölçüm aralığı 
  yaklaşık olarak 2600 mV'ye kadar genişletilir (1V giriş = ADC okuması 
  1575).*/

  /*analogSetPinAttenuation(pin, attenuation1); /*Belirtilen pini için 
  giriş zayıflatmasını ayarlar. Varsayılan değer ADC_11db'dir. 
  Zayıflatma değerleri, önceki işlevde belirtildiği gibi aynıdır.*/

  /*adcAttachPin(pin);/*Bir pini ADC'ye bağlar (ayrıca üzerinde 
  bulunabilecek diğer analog modları da temizler). TRUE veya FALSE 
  sonucunu döndürür.*/
  
  /*adcStart(pin); /*Bağlı pinden ADC dönüşümünü başlatır*/
  
  /*adcBusy(pin); /*Pinden ADC veri yolu üzerindeki dönüşümün şu anda 
  çalışıp çalışmadığını kontrol eder (TRUE veya FALSE değeri döndürür).*/
  
  /*resultadcEnd(pin); /* Dönüşümün sonucunu alır: 16-bit tam sayı değeri 
  döndürür.*/


}

void loop() {
  // Potansiyometre değerini okuma
  potValue = analogRead(potPin);
  Serial.println(potValue);
  delay(500);
}

