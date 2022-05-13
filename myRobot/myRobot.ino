#include <Servo.h>

#define SPEED 120

#define echo 4  //yello
#define trig 3  //green

Servo myservo;
const int servoPin = 2; // orange

const int LeftF = 11; // orange
const int LeftR = 10; // yello
const int RightF = 6; // green
const int RightR = 5; // blue

const int IRl = A2;  //white
const int IRm = A1;  //blue
const int IRr = A0;  //purple



void setup() {
  Serial.begin(9600);  

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  
  pinMode(LeftF, OUTPUT);
  pinMode(RightF, OUTPUT);
  pinMode(LeftR, OUTPUT);
  pinMode(RightR, OUTPUT);

  pinMode(IRl,INPUT);
  pinMode(IRr,INPUT);
  pinMode(IRm,INPUT);

  myservo.attach(servoPin);
}

void loop() 
{ 
  int x = digitalRead(IRl);
  int y = digitalRead(IRm);
  int z = digitalRead(IRr);

  if(x == HIGH && z == LOW)
  { turnLeft();
    delay(10);
  }
  else if(x == LOW &&  z == HIGH)
  { turnRight();
    delay(10);
  }
  else if (x == HIGH &&  z == HIGH && y==HIGH)
  { Stop();
    delay(500);
    turnRight(); 
    delay(100);
    Stop();
    delay(100);     
  }
  else if (x == LOW &&  z == LOW && y==LOW){
      Stop();   
  }
  else
  { Forward();
  }
  
  if(detectObj())
  { turnUSsensorMiddle();
    obstacleDetector();
  }
  else
  { Forward();
  }
  
}

void obstacleDetector()
{
  Stop();
  turnUSsensorMiddle();  
  delay(3000);
  
  if(detectObj())
  { turnUSsensorLeft();
    delay(1000);
    if(detectObj())
    { turnUSsensorMiddle();
      Stop();
      delay(100);
    }
    else
    { offRoadL();
    }
  }
  else
  { Forward();
    delay(100);
  }
}


void Forward()
{ Serial.println("Forward");
  analogWrite(LeftF, SPEED);
  analogWrite(RightF, SPEED);
  analogWrite(LeftR, 0);
  analogWrite(RightR, 0);
}

void turnLeft()
{ Serial.println("Left");
  analogWrite(LeftF, 0);
  analogWrite(LeftR, 0);
  analogWrite(RightR, 0); 
  analogWrite(RightF, SPEED);
}

void turnRight()
{ Serial.println("Right");
  analogWrite(LeftF, SPEED);
  analogWrite(LeftR, 0);
  analogWrite(RightR, 0); 
  analogWrite(RightF, 0);
}

void Reverse()
{ Serial.println("Reverse");
  analogWrite(LeftR, SPEED);
  analogWrite(RightR, SPEED);
  analogWrite(LeftF, 0);
  analogWrite(RightF, 0);
}

void Stop()
{ Serial.println("Stop");
  analogWrite(LeftF, 0);
  analogWrite(LeftR, 0); 
  analogWrite(RightF, 0);
  analogWrite(RightR, 0);
}

void turnUSsensorLeft()
{ Serial.println("sensor turned left");
  myservo.write(135);
}

void turnUSsensorRight()
{  Serial.println("sensor turned right");
   myservo.write(2);
}

void turnUSsensorMiddle()
{ Serial.println("sensor turned middle");
  myservo.write(90);
}

bool detectLineIR()
{ int z = digitalRead(IRr);
  if(z==1)
  { return true;    
  }
  else
  { return false;
  }
}

void offRoadL()
{ turnUSsensorMiddle();
  delay(500);
  while(detectObj())
  { turnLeft();
    delay(50);
    Stop();
    delay(50);
  }
  
  turnLeft();
  delay(50);
  Stop();
  delay(50);
  
  while(detectObjDistance()>20){
    turnUSsensorRight();
    delay(100);
    Forward();
    delay(100); 
    Stop();
    delay(10);   
  }  
  delay(1000);
  turnUSsensorRight();
  delay(500);
  while(!detectLineIRm())
  { if(detectObjDistance()<20)
    { turnLeft();delay(100);
      stop_Stop();delay(50);
      Serial.println(detectObjDistance());
    }
    else if(detectObjDistance()>30)
    { turnRight();delay(100);
      stop_Stop();delay(50);
      Serial.println(detectObjDistance());
    }
    else if(detectObjDistance()>20 && detectObjDistance()<30)
    { Forward();Serial.println(detectObjDistance());
    }
  }turnUSsensorMiddle(); 
}

//void offRoadR()
//{
//  turnUSsensorMiddle();
//  while(detectObj())
//  { turnRight();
//  }
//  turnUSsensorLeft();
//  while(!detectLineIR())
//  { if(detectObjDistance()<20){turnRight();Serial.println(detectObjDistance());}
//    else if(detectObjDistance()>30){turnLeft();Serial.println(detectObjDistance());}
//    else if(detectObjDistance()>20 && detectObjDistance()<30)
//    { Forward();Serial.println(detectObjDistance());
//    }
//  }
//  
//}

bool detectObj()
{ digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  unsigned long t = pulseIn(echo,HIGH); //microseconds
  float d = 340.0*(t/1000000.0)/2 *100; //cm
  Serial.println(d);
  if(d<35)
  { return true;
  }
  else
  { return false;
  }
}

int detectObjDistance()
{ digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  unsigned long t = pulseIn(echo,HIGH); //microseconds
  float d = 340.0*(t/1000000.0)/2 *100; //cm  
  return d;
}
