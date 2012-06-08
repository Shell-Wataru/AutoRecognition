//arduino motor control program

void motorStop();
int power=200;

void changePower(int l){
  analogWrite(5,l); 
}
void setup() {                

  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  changePower(power);
  motorStop();
  Serial.begin(9600);
  Serial.println("start!");
}

void motorGo(){
  digitalWrite(3,HIGH);
  digitalWrite(4,LOW);
}

void motorStop(){
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
}

void motorBack(){
  digitalWrite(3,LOW);
  digitalWrite(4,HIGH);
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
  }
}
