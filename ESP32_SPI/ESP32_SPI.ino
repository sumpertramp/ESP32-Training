/***SUMEYYE DERELLI
------------------------ SPI ILETISIM PROTOKOLU -----------------------------------------
-Mikrodenetleyici: ESP32 
-Platform: Arduino IDE

- SPI, mikroişlemci ile bir veya daha çevrebirimi arasında senkron haberleşme sağlayan seri
iletişim protokolüdür.
- SPI'da çevrebirimi slave olarak kullanılırken mikrodenetleyiciler master olur.
- Veriler aynı anda gönderilebilir veya alınabilir.(full dublex)
- Yalnızda bir adet master kullanılabilir fakat bırden çok slave ile iletişim kurulabilir.
- Slave olarak kullanılan SPI haberleşmeye olanak tanıyan bir sensör, lcd ekran, microSD kart
veya slave olarak programlanacak başka bir mikrodenetleyici olabilir.
- SPI haberleşmesi için 4 adet bağlantıya ihtiyaç vardır. 
       *MISO(Master In Slave Out) -Bazı yerlerde SDO
       *MOSI(Master Out Slave In) -Bazı yerlerde SDI
       *SCK(Serial Clock) 
       *CS/SS: Schip Select -SPI'da birden fazla slave olduğunda cihaz seçmek için kullanılır.
- ESP32, 4 SPI çevre birimini entegre eder: SPI0, SPI1, SPI2 (genellikle HSPI olarak 
adlandırılır) ve SPI3 (genellikle VSPI olarak adlandırılır )
- SP0 ve SP1, yerleşik flash bellekle iletişim kurmak için dahili olarak kullanılır ve 
bunları başka görevler için kullanmamalısınız.
-Diğer cihazlarla iletişim kurmak için HSPI ve VSPI kullanabilirsiniz . HSPI ve VSPI 
bağımsız veri yolu sinyallerine sahiptir ve her veri yolu üç adede kadar SPI kölesi 
çalıştırabilir.
-Varsayılan SPI pinleri:
      SPI	   MOSİ	     MİSO	     SCLK	     CS
      VSPI	 GPIO 23	 GPIO 19	 GPIO 18	 GPIO 5
      HSPI	 GPIO 13	 GPIO 12	 GPIO 14	 GPIO 15
      
Aşağıdaki kod ESP32 ile BME280 sensörünün Adafruit_BME280 library eklenerek SPI yolu ile 
haberleşmesi için yazılmış örnek bir kodtur. */

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include <SPI.h>
#define BME_SCK 25
#define BME_MISO 32
#define BME_MOSI 26
#define BME_CS 33
#define SEALEVELPRESSURE_HPA (1013.25)

//Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

unsigned long delayTime;

void setup() {
  Serial.begin(9600);
  Serial.println(F("-------------------- BME280 TEST --------------------" ));

  bool status;

  // varsayılan ayarlar
  status = bme.begin();  
  if (!status) {
    Serial.println("BME280 sensörü bulunamadı, bağlantıyı kontrol edin!");
    while (1);
  }

  Serial.println("-------------------- Default Test --------------------");
  delayTime = 1000;

  Serial.println();
}


void loop() { 
  printValues();
  delay(delayTime);
}

void printValues() {
  Serial.print("Sıcaklık = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");
  
  // Sıcaklığı Fahrenheit'a çevirme
  /*Serial.print("Temperature = ");
  Serial.print(1.8 * bme.readTemperature() + 32);
  Serial.println(" *F");*/
  
  Serial.print("Basınç = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Yaklaşık Yükseklik = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.print("Nem = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.println();
}
