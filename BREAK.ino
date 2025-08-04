#include <Wire.h> 
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <OneButton.h>

#define EEPROM_SIZE 32

#define SDAPin 8
#define SCLPin 9

#define button1Pin 2
#define button2Pin 1
#define button3Pin 4
#define button4Pin 3

#define buzzerPin 10

bool bootLogo = true;
bool lcdBacklightS = true;


String input = "";

enum InputState {
  NORMAL,
  EEPROM_WAIT_ADDRESS,
  EEPROM_WAIT_VALUE
};
InputState state = NORMAL;
int targetAddress = -1;


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

  pinMode(buzzerPin, OUTPUT);
  
  if (lcdBacklightS == true){lcd.backlight();} else {lcd.noBacklight();}
  if (bootLogo == true){displayBoot();} else {}
  
  while (!Serial) {
    ; 
  }
  
  if (!EEPROM.begin(EEPROM_SIZE)) {
    Serial.println("ERROR:  EEPROM init failed!");
    return;
  }
 
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

  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n' || c == '\r') {
      input.trim();

      if (input.length() > 0) {
        switch (state) {
          case NORMAL:
            processCommand(input);
            break;
          case EEPROM_WAIT_ADDRESS:
            handleEepromAddressInput(input);
            break;
          case EEPROM_WAIT_VALUE:
            handleEepromValueInput(input);
            break;
        }
      } else {
        Serial.println();
      }

      input = "";
      if (state == NORMAL) {
        Serial.print("BREAK# ");
      }
    } else {
      input += c;
    }
  }

    

}


//Button click functions
void click1() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("     19:30");
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
  lcd.print("MEOW");
}

void displayBoot(){
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

void getSettings () {

}

void processCommand(String cmd) {
  if (cmd == "help") {
    printHelp();
  } else if (cmd == "1") {
    Serial.println("Write to EEPROM");
    Serial.println("Address (0-31):");
    state = EEPROM_WAIT_ADDRESS;
  } else if (cmd == "2") {
    listEepromValues();
  } else if (cmd == "3") {
    Serial.println("command 3");
  } else {
    Serial.println("ERROR: Unknown command, use 'help'");
  }
}

void handleEepromAddressInput(String input) {
  int addr = input.toInt();
  if (addr >= 0 && addr < 32) {
    targetAddress = addr;
    Serial.print("Setting the value of address ");
    Serial.print(targetAddress);
    Serial.println(" to (0-255):");
    state = EEPROM_WAIT_VALUE;
  } else {
    Serial.println("Invalid address. The value must be between 0 and 31.");
    state = NORMAL;
  }
}

void handleEepromValueInput(String input) {
  int val = input.toInt();
  if (val >= 0 && val <= 255) {
    EEPROM.write(targetAddress, val);
    EEPROM. commit();
    Serial.print("Wrote  ");
    Serial.print(val);
    Serial.print(" to address ");
    Serial.println(targetAddress);
  } else {
    Serial.println("Invalid value, it must be between 0 and 255.");
  }
  state = NORMAL;
}

void listEepromValues() {
  Serial.println();
  
  const int cols = 4; 
  const int total = 32;
  
  for (int i = 0; i < total; i++) {
    int val = EEPROM.read(i);

    
    Serial.print("AD");
    if (i < 10) Serial.print(" "); 
    Serial.print(i);
    Serial.print(": ");

   
    if (val < 10) {
      Serial.print("  "); 
    } else if (val < 100) {
      Serial.print(" ");  
    }
    Serial.print(val);
    Serial.print("    "); 

    
    if ((i + 1) % cols == 0) {
      Serial.println();
    }
  }
}

void printHelp() {
  Serial.println("Available commands:");
  Serial.println("  help  - List the available commands");
  Serial.println("  1     - Write to EEPROM");
  Serial.println("  2     - Read from the EEPROM");
  Serial.println("  3     - Not used");
}

