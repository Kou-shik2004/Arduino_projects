#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 2  
#define RELAY_PIN 7  
#define TARGET_TEMP 20.0
#define HYSTERESIS 1.0
#define DETECT 4  

DHT dht(DHTPIN, DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2);  

void setup() {
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(DETECT,OUTPUT);
  dht.begin();
  lcd.begin(16, 2);  
  lcd.backlight();  
}

void loop() {
  float temperature = dht.readTemperature();

  if (!isnan(temperature)) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print(" C");

    if (temperature < (TARGET_TEMP - HYSTERESIS)) {
      
      digitalWrite(RELAY_PIN, LOW);
      digitalWrite(DETECT, HIGH);
    } else if (temperature > (TARGET_TEMP + HYSTERESIS)) {
      
      digitalWrite(RELAY_PIN,HIGH);
      digitalWrite(DETECT, LOW);
    }
    
  } else {
    Serial.println("Failed to read temperature from DHT sensor");
  }

  delay(1000);  
}
