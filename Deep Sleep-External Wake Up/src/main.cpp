/*ESP32 Deep Sleeep - External Wake Up

-Zamanlayıcı ve dokunmatik pimlerin yanı sıra, bir düğmeye basmak gibi 
bir pim üzerindeki bir sinyalin değerini değiştirerek ESP32'yi derin 
uykudan uyandırabiliriz. Buna harici uyandırma denir. İki harici 
uyandırma olasılığınız vardır: ext0 ve ext1.

-Bu uyandırma kaynağı, ESP32'yi uyandırmak için bir iğne kullanmanıza izin verir.

ext0 uyandırma kaynağı seçeneği, uyandırmak için RTC GPIO'ları kullanır. 
Bu nedenle, bu uyandırma kaynağı talep edilirse, derin uyku sırasında 
RTC çevre birimleri açık tutulacaktır.

-Bu uyandırma kaynağı, birden fazla RTC GPIO kullanmanıza izin verir. İki farklı mantık işlevini kullanabilirsiniz:

Seçtiğiniz pinlerden herhangi biri yüksekse ESP32'yi uyandırın;
Seçtiğiniz tüm pinler düşükse ESP32'yi uyandırın.
Bu uyandırma kaynağı, RTC denetleyicisi tarafından uygulanır. 
Böylece, RTC çevre birimleri ve RTC bellekleri bu modda kapatılabilir.
*/

#include <Arduino.h>
/*
Donanım Bağlantıları
======================
Push Buton, 10K Ohm dirençle aşağı çekilmiş GPIO 33'e bağlıdır.

NOT:
======
Sadece RTC IO, harici bir uyandırma kaynağı olarak kullanılabilir. Bunlar pimlerdir: 0,2,4,12-15,25-27,32-39.

*/

#define BUTTON_PIN_BITMASK 0x200000000 // 2^33 hex olarak ifade edilmiştir

RTC_DATA_ATTR int bootCount = 0;

/*
ESP32'nin uyku modundan uyandırılma nedenini yazdırmak için kullanılan metod
*/
void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("RTC_IO kullanılarak harici sinyal tarafından uyandırma"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("RTC_CNTL kullanılarak harici sinyal tarafından uyandırma"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Zamanlayıcı tarafından uyandırma"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Dokunmatik panel tarafından uyandırma"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("ULP programı tarafından uyandırma"); break;
    default : Serial.printf("Uyandırma derin uyku nedeniyle gerçekleşmedi: %d\n",wakeup_reason); break;
  }
}

void setup(){
  Serial.begin(115200);
  delay(1000); // Seri Monitör'ün açılması için bir süre beklenir

  // Boot sayısını artır ve her yeniden başlatmada yazdır
  ++bootCount;
  Serial.println("Boot sayısı: " + String(bootCount));

  // ESP32'nin uyandırma nedenini yazdır
  print_wakeup_reason();

  /*
  İlk olarak, uyandırma kaynağını yapılandırırız
  ESP32'yi harici bir tetikleyici için uyandırmak üzere ayarlarız.
  ESP32 için iki tür vardır: ext0 ve ext1.
  ext0, RTC_IO kullanarak uyandırma yapar, bu nedenle RTC bileşenlerinin
  açık olması gereklidir. ext1 ise RTC Denetleyicisini kullanır,
  bu nedenle bileşenlerin güçlendirilmesine ihtiyaç duymaz.
  Dahili pull-up/pull-down kullanmanız durumunda, RTC bileşenlerinin
  açık olması da gereklidir.
  */
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33,1); // 1 = Yüksek, 0 = Düşük

  // ext1 kullanacak olsaydınız, aşağıdaki gibi kullanırdınız
  // esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK,ESP_EXT1_WAKEUP_ANY_HIGH);

  // Şimdi uykuya geç
  Serial.println("Şimdi uykuya geçiyor");
  delay(1000);
  esp_deep_sleep_start();
  Serial.println("Bu asla yazdırılmayacak");
}

void loop(){
  // Bu çağrılmayacak
}