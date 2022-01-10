#include <Keypad.h>
#include <LiquidCrystal.h>


// LCD DEFINITION
const int stepPin = 7; 
const int dirPin = 6;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
String a;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// KEYPAD DEFINITION
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {14, 15, 16, 17}; 
byte colPins[COLS] = {18, 19, 20, 21};
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 


void setup() {
  lcd.begin(16, 2);
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);
}

void loop() {
  char key = customKeypad.getKey();
  if(key!=NO_KEY){
    lcd.leftToRight();
    lcd.print(key);
  }
  if(key=='1'){
    digitalWrite(dirPin,HIGH); // Enables the motor to move in a particular direction
    // Makes 200 pulses for making one full cycle rotation
    for(int x = 0; x < 200; x++) {
      digitalWrite(stepPin,HIGH); 
      delayMicroseconds(500); 
      digitalWrite(stepPin,LOW); 
      delayMicroseconds(500); 
    }
    delay(1000);
  } 
}