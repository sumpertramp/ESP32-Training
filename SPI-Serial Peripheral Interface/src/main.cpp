/*ESP32 SPI

-Bu, Arduino IDE kullanan ESP32 ile SPI iletişim protokolü hakkında basit
bir kılavuzdur. ESP32 SPI pinlerine, SPI cihazlarının nasıl bağlanacağına,
özel SPI pinlerinin nasıl tanımlanacağına, birden fazla SPI cihazının 
nasıl kullanılacağına ve çok daha fazlasına göz atacağız.

-SPI, Serial Peripheral Interface anlamına gelir ve mikrodenetleyiciler 
tarafından bir veya daha fazla çevre birimi ile iletişim kurmak için 
kullanılan senkronize bir seri veri protokolüdür. Örneğin, ESP32 kartınız
SPI'yi destekleyen bir sensörle veya başka bir mikrodenetleyiciyle 
iletişim kurabilir.

-Bir SPI iletişiminde, her zaman çevresel aygıtları (slave olarak da 
adlandırılır) kontrol eden bir denetleyici (ana olarak da adlandırılır) 
vardır. Veriler aynı anda gönderilebilir ve alınabilir. Bu, master'ın 
bir slave'e veri gönderebileceği ve bir slave'in de master'a aynı anda 
veri gönderebileceği anlamına gelir.

-Bir mikrodenetleyici (ESP32) olacak tek bir yöneticiniz olabilir, 
ancak birden çok köleniz olabilir. Köle bir sensör, ekran, microSD kart 
vb. veya başka bir mikrodenetleyici olabilir. Bu, birden fazla sensöre 
bağlı bir ESP32'ye sahip olabileceğiniz, ancak aynı sensörün aynı anda 
birden fazla ESP32 kartına bağlanamayacağı anlamına gelir.

-SPI iletişimi için dört hatta ihtiyacınız vardır:
MISO: Master In Slave Out
MOSI: Master Out Slave Input
SCK: Serial Clock
CS /SS: Yonga Seçimi (aynı SPI veri yolunda birden fazla çevre birimi kullanıldığında aygıtı seçmek için kullanılır)
Sensörler, ekranlar ve diğerleri gibi yalnızca bağımlı bir cihazda farklı bir terminoloji bulabilirsiniz:

MISO, SDO (Serial Data Out) olarak etiketlenebilir
MOSI, SDI (Serial Data In) olarak etiketlenebilir.

-ESP32, 4 SPI çevre birimini entegre eder: SPI0, SPI1, SPI2 (genellikle 
HSPI olarak anılır) ve SPI3 (genellikle VSPI olarak anılır).SP0 ve SP1, 
yerleşik flash bellekle iletişim kurmak için dahili olarak kullanılır ve 
bunları başka görevler için kullanmamalısınız. Diğer cihazlarla iletişim 
kurmak için HSPI ve VSPI kullanabilirsiniz. HSPI ve VSPI bağımsız veri 
yolu sinyallerine sahiptir ve her veri yolu üç adede kadar SPI kölesi 
çalıştırabilir.

-Birçok ESP32 kartı, önceden atanmış varsayılan SPI pinleriyle birlikte 
gelir. Çoğu kart için pin eşleştirmesi aşağıdaki gibidir:
SPI  MOSI    MISO    SCLK    CS
VSPI GPIO 23 GPIO 19 GPIO 18 GPIO 5
HSPI GPIO 13 GPIO 12 GPIO 14 GPIO 15

-Uyarı: Kullandığınız karta bağlı olarak, varsayılan SPI pinleri farklı 
olabilir. Bu nedenle, kullandığınız kartın pin düzenini kontrol 
ettiğinizden emin olun. Ek olarak, bazı kartlarda önceden atanmış SPI 
pinleri yoktur, bu nedenle bunları kod üzerinde ayarlamanız gerekir.

-Not: genellikle, belirtilmediğinde, kart, bir SPI iletişimini varsayılan
ayarlarla başlatırken VSPI pinlerini kullanır. Kartınız önceden atanmış 
pinlerle gelse de gelmese de, bunları her zaman kodla ayarlayabilirsiniz.
*/

/*#include <Arduino.h>

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
} */

/*Önemli: Araçlar > Pano'da kullandığınız panoyu seçtiğinizden emin olun,
aksi takdirde doğru pimleri alamayabilirsiniz. Kodu yükledikten sonra 
Serial Monitor'ü açın, panonuzu RST yapın ve SPI pinlerini göreceksiniz.

-SPI çevre birimlerinizle arayüz oluşturmak için kitaplıkları kullanırken,
özel SPI pimlerini kullanmak genellikle basittir çünkü bunları kitaplık
kurucusuna bağımsız değişken olarak iletebilirsiniz. Örneğin, 
Adafruit_BME280 kitaplığını kullanarak bir BME280 sensörüyle arabirim 
oluşturan aşağıdaki örneğe hızlıca göz atın.*/

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
  Serial.println(F("BME280 test"));

  bool status;

  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  status = bme.begin();  
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  Serial.println("-- Default Test --");
  delayTime = 1000;

  Serial.println();
}


void loop() { 
  printValues();
  delay(delayTime);
}

void printValues() {
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");
  
  // Convert temperature to Fahrenheit
  /*Serial.print("Temperature = ");
  Serial.print(1.8 * bme.readTemperature() + 32);
  Serial.println(" *F");*/
  
  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.println();
}


