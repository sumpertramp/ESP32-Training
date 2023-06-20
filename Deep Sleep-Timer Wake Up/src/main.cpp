/*ESP32 Deep Sleep-Timer Wake Up
-ESP32 Derin Uyku modu içindir. ESP32'yi nasıl derin uykuya sokacağınızı
 göstereceğiz ve onu uyandırmak için farklı modlara göz atacağız:
  -timer wake up, 
  -touch wake up,
  -external wake up
-ESP32 derin uyku moduna geçebilir ve ardından önceden tanımlanmış zaman
dilimlerinde uyanabilir. Bu özellik, düşük güç tüketimini korurken zaman
damgası veya günlük görevler gerektiren projeler yürütüyorsanız 
özellikle kullanışlıdır.
-ESP32 RTC denetleyicisi, önceden tanımlanmış bir süre sonunda ESP32'yi
uyandırmak için kullanabileceğiniz yerleşik bir zamanlayıcıya sahiptir.
*/

#include <Arduino.h>
#include <esp_sleep.h>

#define uS_TO_S_FACTOR 1000000 /* Mikrosaniyeleri saniyelere dönüştürmek için dönüşüm faktörü /
#define TIME_TO_SLEEP 5 / ESP32'nin uyku moduna geçeceği süre (saniye cinsinden) */

RTC_DATA_ATTR int bootCount = 0;

/*
ESP32'nin uyku modundan neden uyandığını ekrana yazdıran metod
*/
void print_wakeup_reason(){
esp_sleep_wakeup_cause_t wakeup_reason;

wakeup_reason = esp_sleep_get_wakeup_cause();

switch(wakeup_reason)
{
case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("RTC_IO kullanılarak harici sinyal ile uyandırma"); break;
case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("RTC_CNTL kullanılarak harici sinyal ile uyandırma"); break;
case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Zamanlayıcı ile uyandırma"); break;
case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Dokunmatik yüzey ile uyandırma"); break;
case ESP_SLEEP_WAKEUP_ULP : Serial.println("ULP programı ile uyandırma"); break;
default : Serial.printf("Uyandırma derin uykudan kaynaklanmıyor: %d\n",wakeup_reason); break;
}
}

void setup(){
Serial.begin(115200);
delay(1000); // Seri Monitör'ün açılması için biraz zaman veriyoruz

// Her başlangıçta bootCount değerini arttırıp ekrana yazdırıyoruz
++bootCount;
Serial.println("Boot sayısı: " + String(bootCount));

// ESP32'nin uyanma nedenini ekrana yazdırıyoruz
print_wakeup_reason();

/*
Uyanma kaynağını ayarlıyoruz
ESP32'yi her 5 saniyede bir uyandırmak için ayar yapıyoruz
*/
esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
Serial.println("ESP32'nin her " + String(TIME_TO_SLEEP) + " saniyede bir uyuması ayarlandı");

/*
Şimdi hangi araçları kapatıp açık bırakacağımıza karar veriyoruz
Varsayılan olarak, ESP32, uyanma kaynağı tarafından ihtiyaç duyulmayan
araçları otomatik olarak kapatır, ancak güç kullanımını kontrol etmek isterseniz
bu kısım size yöneliktir. Ayrıntılı bilgiyi API dokümantasyonunda bulabilirsiniz.
http://esp-idf.readthedocs.io/en/latest/api-reference/system/deep_sleep.html
Aşağıdaki satır, derin uyku sırasında tüm RTC araçlarını kapatır.
*/
//esp_deep_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
//Serial.println("Tüm RTC Araçları, uyku sırasında kapatıldı");

/*
Şimdi, uyanma nedenini ayarladık ve gerekiyorsa derin uykuya
girecek araçların durumunu ayarladık, artık derin uykuya geçebiliriz.
Eğer uyanma kaynağı belirtilmediyse ama derin uyku başlatıldıysa,
donanım sıfırlama olmadığı sürece sonsuza kadar uyur.
*/

Serial.println("Şimdi uyumaya gidiyoruz");
delay(1000);
Serial.flush();
esp_deep_sleep_start();
Serial.println("Bu asla yazdırılmayacak");
}