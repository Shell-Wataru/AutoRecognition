//arduino motor control program

void motorStop();
int turn_powerL=100;
int turn_powerR=100;
int powerL=200;
int powerR=200;

int headPower=100;

void changePower(int l,int r){
  analogWrite(5,l); 
  analogWrite(9,r);
}

void changeHeadPower(int p){
  analogWrite(11,p);
}

void setup() {                

  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT); 
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  
  changePower(powerL,powerR);
  changeHeadPower(headPower);
  motorStop();
  Serial.begin(9600);
  Serial.println("start!");
}

void motorGo_h(){
  digitalWrite(12,HIGH);
  digitalWrite(13,LOW);
}

void motorStop_h(){
  digitalWrite(12,LOW);
  digitalWrite(13,LOW);
}

void motorBack_h(){
  digitalWrite(12,LOW);
  digitalWrite(13,HIGH);
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
  motorStop_l();
  //motorBack_l();
}
void motorRight(){
  changePower(turn_powerL,turn_powerR);
  motorStop_r();
  //motorBack_r();
  motorGo_l();
}

//4桁の数字を読み込む
int readInt(){
  int ans=0;
  int i=0,inp=-1;
  for(i=0;i<4;i++){
    inp=Serial.read();
    if(inp>='0' && inp<='9'){
      ans=ans*10+inp-'0';
    }else{
      break;
    }
  }
  if(i==0 && inp==-1){
    return -1;
  }
  return ans;
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
  }else if(c=='u'){
    motorGo_h();
  }else if(c=='d'){
    motorBack_h();
  }else if(c=='k'){
    motorStop_h();
  }
}
