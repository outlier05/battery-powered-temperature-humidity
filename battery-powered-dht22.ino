#include <LowPower.h>
#include <Wire.h>
//oled ekran ds1306 chip lib
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


//DHT 22 lib
#include <DHT.h>

//oled var girişleri
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

//dht var
#define DHTPIN 2
#define DHTTYPE DHT11

//DHT
DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
//batarya işlemleri
int battery_pin = A3;
const float battery_max = 4.10;
const float battery_min = 3.00;
const float per_bits = 0.004475;

void setup() {
  //debug amaçlı
  analogReference(INTERNAL);
  Serial.begin(9600);
  dht.begin();
  delay(2000);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
  }
  display.clearDisplay();
  ust_kisim();
  higro_termo();
}

void loop() {
  display.clearDisplay();
  delay(1000);
  ust_kisim();
  higro_termo();
  //for (unsigned int i = 0; i < 4; i++) {
 //   LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
 // }
 // delay(2000);
}

void ust_kisim() {
  //üst sarı kısım
  float voltage = 0;
  long sum = 0;
  float output = 0;
  display.setTextSize(1);
  display.setTextColor(WHITE);
  for (int i = 0;i < 500; i++)
  {
    sum += analogRead(battery_pin);
    delayMicroseconds(1000);
  }
  voltage = sum / (float)500;
  voltage = voltage * per_bits;
  //voltage = (voltage / 1023) * 1.1;
  //voltage = round(voltage * 10);
  int percent = map(voltage * 10, battery_min * 10, battery_max * 10, 0, 100);
  if (percent < 10) {
    char dusuk_voltaj[] = "0 sarja tak";
    display.setCursor(0, 0);
    display.print("Batarya bitiyor,sarj edin.");
  }
  else {
    percent = constrain(percent, 10, 100);
    display.setCursor(0, 7);
    display.print("Batarya % ");
    display.print(percent);
  }

}

void higro_termo() {
  //mavi ilk ve ikinci satır7
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  delay(1000);
  display.setCursor(0, 19);
  display.print("Nem     ");
  display.setTextSize(2);
  display.print("%");
  display.print(h);
  display.setCursor(0, 40);
  display.setTextSize(1);
  display.print("Sicaklik  ");
  display.setTextSize(2);
  display.print(t);
  display.setTextSize(1);
  display.print(F("C"));
  /*display.drawPixel(86, 24, WHITE);
    display.drawPixel(87, 24, WHITE);
    display.drawPixel(86, 25, WHITE);
    display.drawPixel(87, 25, WHITE);*/
  display.display();
}
