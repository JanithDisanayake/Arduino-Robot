#include <Servo.h>

Servo myservo;

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
  Serial.begin(9600);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  pinMode(LeftF, OUTPUT);
  pinMode(RightF, OUTPUT);
  pinMode(LeftR, OUTPUT);
  pinMode(RightR, OUTPUT);

  myservo.attach(2); int x = digitalRead(IRl);
  int y = digitalRead(IRr);


  if (detectObj())
  { turnUSsensorMiddle();
    obstacleDetector();
  }
  else
  { Forward();
  }
}

void loop()
{


}

void obstacleDetector()
{
  Stop();
  turnUSsensorMiddle();
  delay(3000);

  if (detectObj())
  { turnUSsensorLeft();
    delay(1000);
    if (detectObj())
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
  analogWrite(LeftF, 100);
  analogWrite(RightF, 100);
  analogWrite(LeftR, 0);
  analogWrite(RightR, 0);
}

void turnLeft()
{ Serial.println("Left");
  analogWrite(LeftF, 0);
  analogWrite(LeftR, 0);
  analogWrite(RightR, 0);
  analogWrite(RightF, 100);
}

void turnRight()
{ Serial.println("Right");
  analogWrite(LeftF, 100);
  analogWrite(LeftR, 0);
  analogWrite(RightR, 0);
  analogWrite(RightF, 0);
}

void Reverse()
{ Serial.println("Reverse");
  analogWrite(LeftR, 150);
  analogWrite(RightR, 150);
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
{ Serial.println("sensor turned right");
  myservo.write(2);
}

void turnUSsensorMiddle()
{ Serial.println("sensor turned middle");
  myservo.write(90);
}

bool detectLineIRm()
{ int z = digitalRead(IRm);
  if (z == 1)
  { return true;
  }
  else
  { return false;
  }
}

void offRoadL()
{ turnUSsensorMiddle();
  delay(500);
  while (detectObj())
  { turnLeft();
    delay(50);
    Stop();
    delay(50);
  }
  while (detectObjDistance() > 20) {
    turnUSsensorRight();
    delay(1000);
    Forward();
    delay(50);
    Stop();
    delay(50);
  }
  delay(1000);
  turnUSsensorRight();
  delay(500);
  while (!detectLineIRm ())
  { if (detectObjDistance() < 20) {
      turnLeft();
      Serial.println(detectObjDistance());
    }
    else if (detectObjDistance() > 30) {
      turnRight();
      Serial.println(detectObjDistance());
    }
    else if (detectObjDistance() > 20 && detectObjDistance() < 30)
    { Forward(); Serial.println(detectObjDistance());
    }
  }
}

void offRoadR()
{
  turnUSsensorMiddle();
  while (detectObj())
  { turnRight();
  }
  turnUSsensorLeft();
  while (!detectLineIRm())
  { if (detectObjDistance() < 20) {
      turnRight();
      Serial.println(detectObjDistance());
    }
    else if (detectObjDistance() > 30) {
      turnLeft();
      Serial.println(detectObjDistance());
    }
    else if (detectObjDistance() > 20 && detectObjDistance() < 30)
    { Forward(); Serial.println(detectObjDistance());
    }
  }

}

bool detectObj()
{ digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  unsigned long t = pulseIn(echo, HIGH); //microseconds
  float d = 340.0 * (t / 1000000.0) / 2 * 100; //cm
  Serial.println(d);
  if (d < 35)
  { return true;
  }
  else
  { return false;
  }
}

int detectObjDistance()
{ digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  unsigned long t = pulseIn(echo, HIGH); //microseconds
  float d = 340.0 * (t / 1000000.0) / 2 * 100; //cm
  return d;
}
