#include <Wire.h> 
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <OneButton.h>

#define EEPROM_SIZE 512

#define SDAPin 8
#define SCLPin 9

#define button1Pin 2
#define button2Pin 1
#define button3Pin 4
#define button4Pin 3

#define buzzerPin 10

bool bootLogo = true;
bool lcdBacklightS = true;

byte stuLogo1[] = {
  B00000,
  B00000,
  B00110,
  B01001,
  B10000,
  B10000,
  B00000,
  B00000
};
byte stuLogo2[] = {
  B00000,
  B00000,
  B00001,
  B00001,
  B10010,
  B01100,
  B00000,
  B00000
};

LiquidCrystal_I2C lcd(0x27,16,2);

OneButton button1(button1Pin, true);
OneButton button2(button2Pin, true);
OneButton button3(button3Pin, true);
OneButton button4(button4Pin, true);


void setup() {
  Serial.begin(9600);
  Wire.begin(SDAPin, SCLPin);
  
  lcd.init();
  
  if (lcdBacklightS == true){lcd.backlight();}
  else {lcd.noBacklight();}
  

  pinMode(buzzerPin, OUTPUT);
  
  if (bootLogo == true){
    lcd.createChar(0, stuLogo1);
    lcd.createChar(1, stuLogo2);

    digitalWrite(buzzerPin, HIGH);
    delay(200);
    digitalWrite(buzzerPin, LOW);

    lcd.setCursor(0,0);
    lcd.print("     BREAK!");
    lcd.setCursor(0,1);
    lcd.print("  Stu Forge ");
    lcd.setCursor(12,1);
    lcd.write(0);
    lcd.setCursor(13,1);
    lcd.write(1);

    delay(2000);
    lcd.clear();
  }
  else {}
  

  
  
  
  if (!EEPROM.begin(EEPROM_SIZE)) {
    Serial.println("EEPROM init failed!");
    return;
  }
 
  int someValue = 42;
  EEPROM.write(0, someValue); 
  EEPROM.commit(); 


  int readValue = EEPROM.read(0);
  Serial.print("Read: ");
  Serial.println(readValue);

  button1.attachClick(click1);
  button2.attachClick(click2);
  button3.attachClick(click3);
  button4.attachClick(click4);


}

void loop() {
  button1.tick();
  button2.tick();
  button3.tick();
  button4.tick();

}


void getSettings () {

}


//Button click functions
void click1() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Brick");
}

void click2() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Red");
}


void click3() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Csiki csuki");
}


void click4() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("");
}
