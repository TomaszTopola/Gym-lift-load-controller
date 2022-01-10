/**
 * Stepper controler with keypad with voice recognition module integration.
 * Designed for devices with A4988 stepper motor controller, 4x4 matrix keypad and 16x2 LCD.
 * author: Tomasz Topoła (https://github.com/TomaszTopola)
 * comissioned by: Karol Sowa
*/


#include <Keypad.h>
#include <LiquidCrystal.h>

// STEPPER DEFINITION
const int stepperCount = 2;
const int stepPin[stepperCount] = [7, 9]; 
const int dirPin[stepperCount] = [6, 8];

// LCD DEFINITION
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
String a;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// KEYPAD DEFINITION
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},        //A STANDS FOR APPLY
  {'4','5','6','B'},        //B STANDS FOR BACKSPACE
  {'7','8','9','C'},        //C STANDS FOR CLEAR
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {14, 15, 16, 17}; 
byte colPins[COLS] = {18, 19, 20, 21};
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

// OTHER PINOUTS:
const int limitSwitchPin = 15; //TODO IMPORTANT! INSERT VALID PIN NUMBER AND REMOVE THIS COMMENT BEFORE LAUNCHING!

/**
 * Applies motion on a stepper motor.
 * param pulses: amount of pulses sent to stepper motor (200 for one full rotation)
 * param clockwise: if set to true motor turns clockwise, else counter-clockwise
 * param stepperID: contains id of stepper (0 or 1)
*/
void stepperMove(int pulses, bool clockwise, int stepperID){
    // if( stepperID >= stepperCount){
    //     return; //stops program from continuing in case someone provided invalid stepper ID
    // }
    if(clockwise){
        digitalWrite(dirPin[stepperID], HIGH);
    }else{
        digitalWrite(dirPin[stepperID], LOW);
    }
    for(int x = 0; x < pulses; x++) {
        digitalWrite(stepPin[stepperID],HIGH); 
        delayMicroseconds(500); 
        digitalWrite(stepPin[stepperID],LOW); 
        delayMicroseconds(500); 
    }
}

void displayManipulatedChangeIndicator(){
    lcd.setCursor(0,0);
    lcd.print(manipulatedChagneIndicator);
}

void initialStepperCalibration(){
    while(digitalRead(limitSwitchPin, HIGH)){
        stepperMove(1, true, 0);    //TODO REQUIRES VERIFICATION OF DIRECTION AND STEPPER ID
    }
}

void setup() {
    // HARDWARE SETUP
        //PINMODES
        lcd.begin(16, 2);
        lcd.leftToRight();
        for(int i=0; i<stepperCount; i++){
            pinMode(stepPin[i], OUTPUT);
            pinMode(dirPin[i], OUTPUT);
        }
        pinMode(limitSwitchPin, INPUT_PULLUP);
        //VOICE RECOGNITION MODULE AND SERIAL COM
        Serial.begin(9600);
        Serial.write(0xAA);
        Serial.write(0x37);
        delay(1000);
        Serial.write(0xAA);
        Serial.write(0x21);
    // CALIBRATION
    initialStepperCalibration();
}

int currentChangeIndicator = 0;     //the one that is applied and used with any motor operations
int manipulatedChagneIndicator = 0; //the one that stores user input before acceptation
void loop() {
    char key = customKeypad.getKey();
    if(key!=NO_KEY){
        if(key > 48 && key < 57){
            //this section uses ASCII values to recognise user input and transform it into a number that is later used for motor movements
            manipulatedChagneIndicator = manipulatedChagneIndicator*10 + (key-48);
            displayManipulatedChangeIndicator();
        }else if(key=='A'){
            //apply
            lcd.setCursor(1,0)
            lcd.print("CURRENT: " + currentChangeIndicator)
        }else if(key=='B'){
            //backspace
            manipulatedChagneIndicator /= 10;
        }else if(key=='C'){
            //clear
            manipulatedChagneIndicator = 0;
            displayManipulatedChangeIndicator();
        }
    }
}