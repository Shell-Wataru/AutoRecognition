void motorStop_r();
void motorStop_l();

void setup() {                

  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT); 
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  analogWrite(3,0); 
  analogWrite(9,0);
  motorStop_l();
  motorStop_r();

}

void motorGo_r(){
  digitalWrite(7,HIGH);
  digitalWrite(8,LOW);
}

void motorStop_r(){
  digitalWrite(7,LOW);
  digitalWrite(8,LOW);
}

void motorBack_r(){
  digitalWrite(7,LOW);
  digitalWrite(8,HIGH);
}

void motorGo_l(){
  digitalWrite(1,HIGH);
  digitalWrite(2,LOW);
}

void motorStop_l(){
  digitalWrite(1,LOW);
  digitalWrite(2,LOW);
}

void motorBack_l(){
  digitalWrite(1,LOW);
  digitalWrite(2,HIGH);
}

void motor_Go(){
    motorGo_r();
    motorGo_l();
}

void motor_Back(){
    motorBack_r();
    motorBack_l();
}
void motor_Stop(){
    motorStop_r();
    motorStop_l();
}
void motor_Left(){
  motorGo_r();
  motorStop_l();
}
void motor_Right(){
  motorStop_r();
  motorGo_l();
}


void loop() {
  // wait for a second
  delay(3000);
  analogWrite(3,200); 
  analogWrite(9,200);
  motorGo();
  delay(3000);
  motorBack();
  delay(3000);
  motorLeft();
  delay(3000);
  motorRight();
  delay(3000);
  
  motorStop();
  delay(3000);
}
