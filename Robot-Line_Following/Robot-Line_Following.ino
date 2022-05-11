
const int LeftF = 11; // orange
const int LeftR = 10; // yello
const int RightF = 6; // green
const int RightR = 5; // blue

const int IRl = 9;  //white
const int IRr = 8;  //purple
const int IRm = 7;  //blue

#define echo 4  //yello
#define trig 3  //green

void setup() {
  pinMode(LeftF, OUTPUT);
  pinMode(RightF, OUTPUT);
  pinMode(LeftR, OUTPUT);
  pinMode(RightR, OUTPUT);

  pinMode(IRl,INPUT);
  pinMode(IRr,INPUT);
  pinMode(IRm,INPUT);

  Serial.begin(9600);  
}

void loop() {
  int x = digitalRead(IRl);
  int y = digitalRead(IRr);
  int z = digitalRead(IRm);

  if(digitalRead(IRl)==HIGH && digitalRead(IRr)==HIGH) //IR will not glow on black line
  {
    Stop();
  }

  else if(digitalRead(IRl)==LOW && digitalRead(IRr)==LOW)  //IR not on black line
  {
    Forward();
  }

  else if(digitalRead(IRl)==LOW && digitalRead(IRr)==HIGH)
  {
    turnRight();
    delay(10);
  }

  else if(digitalRead(IRl)==HIGH && digitalRead(IRr)==LOW)
  {
    turnLeft();
    delay(10);
  }

  else
  {
    Stop();
  }
}


void Forward()
{
    Serial.println("Forward");
  analogWrite(LeftF, 100);
  analogWrite(RightF, 100);
  analogWrite(LeftR, 0);
  analogWrite(RightR, 0);
}

void Reverse()
{
  Serial.println("Reverse");
  analogWrite(LeftR, 100);
  analogWrite(RightR, 100);
  analogWrite(LeftF, 0);
  analogWrite(RightF, 0);
}

void turnRight()
{
  
  Serial.println("Right");
  analogWrite(LeftF, 100);
  analogWrite(LeftR, 0);
  analogWrite(RightR, 0); 
  analogWrite(RightF, 0);
  
}

void turnLeft()
{
  
  Serial.println("Left");
  analogWrite(LeftF, 0);
  analogWrite(LeftR, 0);
  analogWrite(RightR, 0); 
  analogWrite(RightF, 100);
}


void Stop()
{
  Serial.println("Stop");
  analogWrite(LeftF, 0);
  analogWrite(LeftR, 0); 
  analogWrite(RightF, 0);
  analogWrite(RightR, 0);
}
