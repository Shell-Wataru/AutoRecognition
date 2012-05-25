//arduino motor control program

void motorStop();
int turn_powerL=50;
int turn_powerR=62;
int powerL=50;
int powerR=50;

void changePower(int l,int r){
  analogWrite(5,l); 
  analogWrite(9,r);
}
void setup() {                

  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT); 
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);

  
  changePower(powerL,powerR);
  motorStop();
  Serial.begin(9600);
  Serial.println("start!");
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
  digitalWrite(3,HIGH);
  digitalWrite(4,LOW);
}

void motorStop_l(){
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
}

void motorBack_l(){
  digitalWrite(3,LOW);
  digitalWrite(4,HIGH);
}

void motorGo(){
  changePower(powerL,powerR);
    motorGo_r();
    motorGo_l();
}

void motorBack(){
  changePower(powerL,powerR);
    motorBack_r();
    motorBack_l();
}
void motorStop(){
    motorStop_r();
    motorStop_l();
}



void motorLeft(){
  changePower(turn_powerL,turn_powerR);
  motorGo_r();
  motorBack_l();
}
void motorRight(){
  changePower(turn_powerL,turn_powerR);
  motorBack_r();
  motorGo_l();
}



void loop() {
  // wait for a second
  int c;
  c=Serial.read();
  if(c>='a' && c<='z'){
    Serial.println(c);
  }
  if(c=='g'){
    motorGo();
  }else if(c=='b'){
    motorBack();
  }else if(c=='s'){
    motorStop();
  }else if(c=='l'){
    motorLeft();
  }else if(c=='r'){
    motorRight();
  }
}
