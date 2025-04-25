//Intro to ABE 
//Data Logging using Arduino Uno
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SD.h>

// DHT setup
#define DHTPIN 2       // DHT11 data pin
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change address if needed (0x27 or 0x3F)

// SD card setup
#define CSPIN 10
File dataFile;

void setup() {
  Serial.begin(9600);
 
  // DHT init
  dht.begin();

  // LCD init
  lcd.init();
  lcd.backlight();
  lcd.print("Initializing...");

  // SD init
  if (!SD.begin(CSPIN)) {
    Serial.println("SD card failed!");
    lcd.setCursor(0, 1);
    lcd.print("SD Failed     ");
  } else {
    Serial.println("SD card ready.");
    lcd.setCursor(0, 1);
    lcd.print("SD Ready      ");
  }

  delay(2000);
  lcd.clear();
}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Failed to read from DHT!");
    return;
  }

  // Display on LCD
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp, 1);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Hum:  ");
  lcd.print(hum, 1);
  lcd.print(" %");

  // Log to SD card
  dataFile = SD.open("climate.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print("Temperature: ");
    dataFile.print(temp);
    dataFile.print(" C, Humidity: ");
    dataFile.print(hum);
    dataFile.println(" %");
    dataFile.close();
    Serial.println("Data logged.");
  } else {
    Serial.println("Error opening climate.txt");
  }

  delay(15000); // Wait 15 seconds before next reading
}
