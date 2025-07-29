#include <Wire.h> 
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>

#define EEPROM_SIZE 512

#define SDAPin 8
#define SCLPin 9

#define buzzerPin 10

LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  Serial.begin(9600);
  Wire.begin(SDAPin, SCLPin);
  
  lcd.init();
  lcd.backlight();

  pinMode(buzzerPin, OUTPUT);
  
  digitalWrite(buzzerPin, HIGH);
  delay(200);
   digitalWrite(buzzerPin, LOW);

  
  lcd.setCursor(0,0);
  lcd.print("BREAK!");
  
  if (!EEPROM.begin(EEPROM_SIZE)) {
    Serial.println("EEPROM init failed!");
    return;
  }
 
  int someValue = 42;
  EEPROM.write(0, someValue); 
  EEPROM.commit(); 

  int readValue = EEPROM.read(0);
  Serial.print("value: ");
  Serial.println(readValue);



}

void loop() {
  
}
