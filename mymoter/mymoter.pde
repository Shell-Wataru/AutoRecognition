/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */

void setup() {                
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT); 
 
}

void motorGo(){
  digitalWrite(1,HIGH);
  digitalWrite(2,LOW);
}

void motorStop(){
  digitalWrite(1,LOW);
  digitalWrite(2,LOW);
}

void motorBack(){
  digitalWrite(1,LOW);
  digitalWrite(2,HIGH);
}

void loop() {
  // wait for a second
  analogWrite(3,200); 
 
  motorStop();
  delay(3000);
 // motorGo();
  motorBack();
  delay(1000);
  motorGo();
  delay(1000);
}
