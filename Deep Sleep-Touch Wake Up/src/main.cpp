#include <Arduino.h>
#define Threshold 40 /* Değer ne kadar büyükse, hassasiyet o kadar yüksek olur */

RTC_DATA_ATTR int bootCount = 0;
touch_pad_t touchPin;
/*
ESP32'nin uykudan uyandırılma sebeplerini ekrana yazdıran metod
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

/*
ESP32'nin uykudan uyandırılma sebeplerini ekrana yazdıran metod
*/
void print_wakeup_touchpad(){
  touchPin = esp_sleep_get_touchpad_wakeup_status();

  switch(touchPin)
  {
    case 0  : Serial.println("GPIO 4 üzerinde dokunma algılandı"); break;
    case 1  : Serial.println("GPIO 0 üzerinde dokunma algılandı"); break;
    case 2  : Serial.println("GPIO 2 üzerinde dokunma algılandı"); break;
    case 3  : Serial.println("GPIO 15 üzerinde dokunma algılandı"); break;
    case 4  : Serial.println("GPIO 13 üzerinde dokunma algılandı"); break;
    case 5  : Serial.println("GPIO 12 üzerinde dokunma algılandı"); break;
    case 6  : Serial.println("GPIO 14 üzerinde dokunma algılandı"); break;
    case 7  : Serial.println("GPIO 27 üzerinde dokunma algılandı"); break;
    case 8  : Serial.println("GPIO 33 üzerinde dokunma algılandı"); break;
    case 9  : Serial.println("GPIO 32 üzerinde dokunma algılandı"); break;
    default : Serial.println("Dokunma algılaması ile uyanma değil"); break;
  }
}

void callback(){
  //boş callback fonksiyonu
}

void setup(){
  Serial.begin(115200);
  delay(1000); // Seri Monitör'ün açılması için biraz zaman veriyoruz

  // Her başlangıçta bootCount değerini arttırıp ekrana yazdırıyoruz
  ++bootCount;
  Serial.println("Boot sayısı: " + String(bootCount));

  // ESP32'nin uyandığı nedeni ve dokunmatik yüzeyi ekrana yazdırıyoruz
  print_wakeup_reason();
  print_wakeup_touchpad();

  // Dokunmatik Yüzey 3 (GPIO15) üzerinde kesmeyi ayarla
  touchAttachInterrupt(T3, callback, Threshold);

  // Dokunmatik yüzeyi uyanma kaynağı olarak ayarla
  esp_sleep_enable_touchpad_wakeup();

  // Şimdi uyku moduna geç
  Serial.println("Şimdi uyumaya gidiyoruz");
  esp_deep_sleep_start();
  Serial.println("Bu asla yazdırılmayacak");
}

void loop(){

}