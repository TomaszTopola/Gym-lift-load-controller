byte com = 0;

// STEPPER DEFINITION
const int stepPin = [7, 9]; 
const int dirPin = [6, 8];

void setup() {
  
Serial.begin(9600);
Serial.write(0xAA);
Serial.write(0x37);
delay(1000);
Serial.write(0xAA);
Serial.write(0x21);
pinMode(stepPin1,OUTPUT); 
pinMode(dirPin1,OUTPUT);
pinMode(stepPin2,OUTPUT); 
pinMode(dirPin2,OUTPUT);
}

void loop() {
  while(Serial.available()) {
    com = Serial.read();
    switch(com) {
      case 0x11:   //command 1 is for Green LED 
        digitalWrite(dirPin1,HIGH); // Enables the motor to move in a particular direction
        // Makes 200 pulses for making one full cycle rotation
        for(int x = 0; x < 200; x++) {
          digitalWrite(stepPin1,HIGH); 
          delayMicroseconds(500); 
          digitalWrite(stepPin1,LOW); 
          delayMicroseconds(500); 
        }
        delay(1000);
        digitalWrite(dirPin1,LOW); //Changes the rotations direction
        // Makes 400 pulses for making two full cycle rotation
        for(int x = 0; x < 200; x++) {
          digitalWrite(stepPin1,HIGH);
          delayMicroseconds(500);
          digitalWrite(stepPin1,LOW);
          delayMicroseconds(500);
        }
        delay(2000);
        break;
      case 0x12:   //command 1 is for Green LED 
        digitalWrite(dirPin2,HIGH); // Enables the motor to move in a particular direction
        // Makes 200 pulses for making one full cycle rotation
        for(int x = 0; x < 200; x++) {
          digitalWrite(stepPin2,HIGH); 
          delayMicroseconds(500); 
          digitalWrite(stepPin2,LOW); 
          delayMicroseconds(500); 
        }
        delay(1000); // One second delay
        digitalWrite(dirPin2,LOW); //Changes the rotations direction
        // Makes 400 pulses for making two full cycle rotation
        for(int x = 0; x < 200; x++) {
          digitalWrite(stepPin2,HIGH);
          delayMicroseconds(500);
          digitalWrite(stepPin2,LOW);
          delayMicroseconds(500);
        }
        delay(2000);
        break; 
    }
  }
}