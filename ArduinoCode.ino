#include <LiquidCrystal.h>
#include <EEPROM.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN1 6
#define DHTPIN2 7
#define DHTTYPE DHT11
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

struct package
{
  float temperature = 0;
  float humidity = 0;
};

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2, lcdcon = 9;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int apin = A0, ypin = A1, fanpin = 10, lcdset = 0;

int icsicaklik, nem, x, y, i, motor, nemset = 65, sicaklikset, set, ypindurumu, apindurumu, nemadres = 10, sicaklikadres = 15;

int dissicaklik, disnem, motorsure, sure, motorstart, motorstop, fansure, bekleme, beklemesure, anakontrol, fannemsure;

float adc = 0;

void setup() {

  //Pin bağlantıları
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(apin, INPUT);
  pinMode(ypin, INPUT);
  pinMode(fanpin, OUTPUT);
  pinMode(lcdcon, OUTPUT);
  // SENSOR BİLGİLERİ
  dht1.begin();
  dht2.begin();

  //Eprom
  nemset = EEPROM.read(nemadres);
  sicaklikset = EEPROM.read(sicaklikadres);
  //LCD HABERLEŞME
  lcd.begin(16, 2);
  analogWrite(lcdcon, 70);
  lcd.print("Sistem Hazir");
  delay(2000);
  lcd.clear();
}

void loop() {

  delay(45);
  digitalWrite(LED_BUILTIN, HIGH);
  nemsicakliksensor();
  button();
  LCD();
  Fan();
  Hafiza();
  HesaplamaOran();
  delay(45);
  digitalWrite(LED_BUILTIN, LOW);
}

void button() {
  
  ypindurumu = digitalRead(ypin);
  apindurumu = digitalRead(apin);

  if (apindurumu == HIGH && ypindurumu == HIGH) {
    lcd.clear();
    set++;
    lcdset++;
    delay(100);
  }
  
  if (set == 1) {
    ypindurumu = digitalRead(ypin);
    apindurumu = digitalRead(apin);
    if (apindurumu == HIGH) {
      sicaklikset++;
    }
    if (ypindurumu == HIGH) {
      sicaklikset--;
    }
  }
  if (set == 2) {
    ypindurumu = digitalRead(ypin);
    apindurumu = digitalRead(apin);
    if (apindurumu == HIGH) {
      nemset++;
    }
    if (ypindurumu == HIGH) {
      nemset--;
    }
  }
  if (set >= 3) set = 0;
  if (lcdset >= 3) lcdset = 0;
}

void nemsicakliksensor() {

  icsicaklik = dht1.readTemperature();
  nem = dht1.readHumidity();

  dissicaklik =  dht2.readTemperature();
  disnem = dht2.readHumidity();
}


void LCD() {

  x++;
  if (lcdset == 1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print ("Sicaklik Degeri");
    lcd.setCursor(0, 1);
    lcd.print(sicaklikset);
  }
  if (lcdset == 2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print ("Nem Degeri");
    lcd.setCursor(0, 1);
    lcd.print(nemset);
  }
  if (lcdset == 0) {

    if (x <= 15) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sicaklik : ");
      lcd.setCursor(12, 0);
      lcd.print(icsicaklik);
      lcd.setCursor(14, 0);
      lcd.print("C");
    }

    if (x >= 16 && x <= 31) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Dis Sicaklik:");
      lcd.setCursor(15, 0);
      lcd.print("C");
      lcd.setCursor(13, 0);
      lcd.print(dissicaklik);

    }
    if (x >= 32 && x <= 47) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Nem : ");
      lcd.setCursor(7, 0);
      lcd.print("%");
      lcd.setCursor(9, 0);
      lcd.print(nem);
    }

    if (x >= 48 && x <= 63) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Dis Nem : ");
      lcd.setCursor(11, 0);
      lcd.print("%");
      lcd.setCursor(12, 0);
      lcd.print(disnem);
    }
    
    if (y == 1) {
      lcd.setCursor(0, 16);
      lcd.print("Fan Calisiyor ");
    }
    
    if (y == 2) {
      lcd.setCursor(0, 16);
      lcd.print("Fan Calismiyor ");
    }
    
    if (y == 3) {
      lcd.setCursor(0, 16);
      lcd.print("Fan Bekliyor ");
    }
    
    if (x >= 64) x = 0;
  }
}

void Fan() { //y değerleri LCD için.

  if (nem >= nemset)  motorstart = 1;
  if (motorstart == 1 && bekleme == 0) {

    if (fansure <= sure) {
      fansure++;

      digitalWrite(fanpin, LOW);
      y = 1;
      beklemesure = 0;
    }
    else {

      digitalWrite(fanpin, HIGH);
      motorstart = 0;
      bekleme = 1;
      y = 2;
    }
  }
  if (bekleme == 1) {
    if (beklemesure <= 600) {
      beklemesure++;
      fansure = 0;
      y = 3;

    }
    else {
      y = 2;

      bekleme = 0;
      motorstart = 1;

    }
  }
}

void HesaplamaOran() {

  if (dissicaklik <= 15) {
    switch (dissicaklik) {
      default: sure = 300; break;
      case 0:  sure = 300; break;
      case 1:  sure = 300; break;
      case 2:  sure = 400; break;
      case 3:  sure = 400; break;
      case 4:  sure = 600; break;
      case 5:  sure = 800; break;
      case 6:  sure = 1000; break;
      case 7:  sure = 1200; break;
      case 8:  sure = 1400; break;
      case 9:  sure = 1600; break;
      case 10: sure = 1800; break;
      case 11: sure = 2000; break;
      case 12: sure = 2200; break;
      case 13: sure = 2400; break;
      case 14: sure = 2400; break;
      case 15: sure = 2400; break;


    }
  }
  if (dissicaklik >= 16) {
    if (anakontrol == HIGH) sure = 2400;
    else  anakontrol = LOW;
  }

}
void Hafiza() {

  EEPROM.write(nemadres, nemset);
  EEPROM.write(sicaklikadres, sicaklikset);


}
