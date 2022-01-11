/**
 * Stepper controler with keypad with voice recognition module integration.
 * Designed for devices with A4988 stepper motor controller, 4x4 matrix keypad and 16x2 LCD.
 * author: Tomasz Topoła (https://github.com/TomaszTopola)
 * comissioned by: Karol Sowa
*/

#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

// STEPPER DEFINITION
const int stepperCount = 2;
const int stepPin[stepperCount] = {7, 9}; 
const int dirPin[stepperCount] = {6, 8};
const int limitSwitchPin[stepperCount] = {15, 16}; //TODO IMPORTANT! INSERT VALID PIN NUMBER AND REMOVE THIS COMMENT BEFORE LAUNCHING!

// STEPPER CALIBRATION VARIABLES
const int positionStepDifference = 200; //defines step distance between two positions.
const int lockStepDifference = 200;     //defines step distance required to lock device on position
const int scale = 5;
const int maxPosition = 12;
int currentLoad = 0;
int currentPosition = 0;

// LCD DEFINITION
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

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

/**
 * Applies motion on a stepper motor.
 * param pulses: amount of pulses sent to stepper motor (200 for one full rotation)
 * param clockwise: if set to true motor turns clockwise, else counter-clockwise
 * param stepperID: contains id of stepper (0 or 1)
*/
void stepperMove(int pulses, bool clockwise, int stepperID){
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

/**
 * Sets steppers to position 0,0
 */
void initialStepperCalibration(){
    while(digitalRead(limitSwitchPin[1]) == HIGH){
        stepperMove(1, false, 1);       //TODO: validate direction
    }
    while(digitalRead(limitSwitchPin[0]) == HIGH){
        stepperMove(1, true, 0);
    }
}

void readKeypad(){
    char key = customKeypad.getKey();
    if(key!=NO_KEY){
        if(key > 48 && key < 57){
            //this section uses ASCII values to recognise user input and transform 
            //it into a number that is later used for motor movements
            currentLoad = currentLoad*10 + (key-48);
        }else if(key=='A'){
            initialStepperCalibration();
            currentPosition = currentLoad/scale;
            setPosition();
        }else if(key=='B'){
            currentLoad /= 10;
        }else if (key=='C'){
            currentLoad = 0;
        }
        lcd.setCursor(0,0);
        lcd.print(currentLoad + " kg");
    }
}


void setPosition(){             //WARNING! uses global variable - cureentPosition
    initialStepperCalibration();
    if(currentPosition == 0){
        return;
    }
    stepperMove(currentPosition*positionStepDifference, false, 0);
    stepperMove(lockStepDifference, true, 1);
}

/**
 * moves device position by selected value
 * param: difference - how many positions to move
 * param: up - true for moving up and false for moving down
 */
void movePosition(int difference){
    int futurePosition = currentPosition + difference;
    if( futurePosition > maxPosition || futurePosition < 1){
        return;     //add extra behaviour on limit cross here.
    }
    if(difference == 0){
        return;
    }
    bool up;
    if(difference > 0){
        up = false;
    }else{
        up = true;
        difference = -difference;
    }
    stepperMove(lockStepDifference, false, 1);
    stepperMove(positionStepDifference*difference, up, 0);
    stepperMove(lockStepDifference, true, 1);
    currentPosition = futurePosition;
}

void setup() {
    // HARDWARE SETUP
        //PINMODES
        lcd.init();
        lcd.backlight();
        for(int i=0; i<stepperCount; i++){
            pinMode(stepPin[i], OUTPUT);
            pinMode(dirPin[i], OUTPUT);
            pinMode(limitSwitchPin[i], INPUT_PULLUP);
        }                 
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

void loop() {
    readKeypad();
    
    byte com = Serial.read();
    switch(com){
        case 0x11:
            movePosition(1);
            break;
        case 0x12:
            movePosition(-1);
            break;
    }
}