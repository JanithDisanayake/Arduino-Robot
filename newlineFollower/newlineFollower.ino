#include <Servo.h>
#include <NewPing.h>

#define speedPinR 5
#define RightMotorDirPin1 4
#define RightMotorDirPin2 7
#define speedPinL 6
#define LeftMotorDirPin1 8
#define LeftMotorDirPin2 12

#define SERVO_PIN 10

#define Echo_PIN 2
#define Trig_PIN 3

#define SPEED1 130
#define SPEED2 110
#define SPEED3 100

#define max_dist 150
#define stop_range 10
#define stop_wait 4

#define IRSensorL A2
#define IRSensorM A1
#define IRSensorR A0

#define buzzer 13

int cm;
bool turning;
int counter;


NewPing sonar(Trig_PIN, Echo_PIN, max_dist);
Servo servo;

void setup() {

  pinMode(RightMotorDirPin1, OUTPUT);
  pinMode(RightMotorDirPin2, OUTPUT);
  pinMode(speedPinL, OUTPUT);

  pinMode(LeftMotorDirPin1, OUTPUT);
  pinMode(LeftMotorDirPin2, OUTPUT);
  pinMode(speedPinR, OUTPUT);

  pinMode(Trig_PIN, OUTPUT);
  pinMode(Echo_PIN, INPUT);

  pinMode(IRSensorL, INPUT);
  pinMode(IRSensorM, INPUT);
  pinMode(IRSensorR, INPUT);

  pinMode(buzzer, OUTPUT);


  servo.attach(SERVO_PIN);
  servo.write(85);
  delay(200);

  Serial.begin(9600);

  stop_Stop();
  counter = 0;
}

void loop() {
  
  
  int counter2 = 0;
  cm = readDistance();

  Serial.println(cm);

  if (cm <= stop_range && cm != 0) {
    digitalWrite(buzzer, HIGH);
    delay(500);
    digitalWrite(buzzer, LOW);
    counter++;
  }

  if (counter >= stop_wait) {
    counter = 0;
    stop_Stop();
    turning = true;

    go_Right();
    delay(380);
    stop_Stop();
    delay(500);
    servo.write(0);
    delay(500);

    while (turning) {
      delay(100);
      cm = readDistance();
      if (cm < 20 && cm != 0) {
        go_Advance();
        delay(50);
        stop_Stop();
      }
      else {
        counter2++;
      }
      if (counter2 > 10) {
        go_Advance();
        delay(500);
        stop_Stop();
        turning = false;
      }
    }

    go_Left();
    delay(310);
    go_Advance();
    delay(500);
    stop_Stop();
    delay(500);
    turning = true;
    counter2 = 0;

    while (turning) {
      delay(100);
      cm = readDistance();
      if (cm < 20 && cm != 0) {
        go_Advance();
        delay(50);
        stop_Stop();
      }
      else {
        counter2++;
      }
      if (counter2 > 10) {
        go_Advance();
        delay(500);
        stop_Stop();
        turning = false;
      }
    }
    go_Left();
    delay(305);
    stop_Stop();
    delay(500);
    turning = true;

    while (turning) {
      int valueLeft = digitalRead(IRSensorL);
      int valueRight = digitalRead(IRSensorR);
      if (valueLeft == LOW || valueRight == LOW) {
        go_Advance();
        delay(50);
        stop_Stop();
      } else { // line found
        turning = false;
        go_Right();
        delay(300);
        stop_Stop();
        servo.write(103);
      }
    }
  } else {
    followLine();
  }
}


void followLine() {

  int valueLeft = digitalRead(IRSensorL);
  int valueRight = digitalRead(IRSensorR);
  int valueMiddle = digitalRead(IRSensorM);

  if ((valueLeft == HIGH && valueRight == LOW )) {
    go_Left();
  } else if ((valueLeft == LOW &&  valueRight == HIGH)) {
    go_Right();
  } else if (valueLeft == HIGH &&  valueRight == HIGH && valueMiddle == HIGH) {
    go_Right();
  } else if (valueLeft == LOW &&  valueRight == LOW && valueMiddle == LOW) {
    turn_back();
  }
  else {
    go_Advance();
  }
}

//Set Motor Speed
void set_Motorspeed(char g) {
  if (g == 'L' || g == 'R' || g == 'S') {
    analogWrite(speedPinL, SPEED2);
    analogWrite(speedPinR, SPEED2);
  }else if(g == 'T'){
    analogWrite(speedPinL, SPEED3);
    analogWrite(speedPinR, SPEED3);
  }
  else {
    analogWrite(speedPinL, SPEED1);
    analogWrite(speedPinR, SPEED1);
  }

}

//Read Ultrasonic Sensor
int readDistance() {
  int cm = sonar.ping_cm();
  return cm;
}

void go_Advance() {
  digitalWrite(RightMotorDirPin1, LOW);
  digitalWrite(RightMotorDirPin2, HIGH);
  digitalWrite(LeftMotorDirPin1, LOW);
  digitalWrite(LeftMotorDirPin2, HIGH);
  set_Motorspeed('A');
}
void go_Right() {
  digitalWrite(RightMotorDirPin1, LOW);
  digitalWrite(RightMotorDirPin2, HIGH);
  digitalWrite(LeftMotorDirPin1, HIGH);
  digitalWrite(LeftMotorDirPin2, LOW);
  set_Motorspeed('R');
}

void go_Left() {
  digitalWrite(RightMotorDirPin1, HIGH);
  digitalWrite(RightMotorDirPin2, LOW);
  digitalWrite(LeftMotorDirPin1, LOW);
  digitalWrite(LeftMotorDirPin2, HIGH);
  set_Motorspeed('L');
}

void stop_Stop() {
  digitalWrite(RightMotorDirPin1, LOW);
  digitalWrite(RightMotorDirPin2, LOW);
  digitalWrite(LeftMotorDirPin1, LOW);
  digitalWrite(LeftMotorDirPin2, LOW);
  set_Motorspeed('S');
}

void turn_back() {
  digitalWrite(RightMotorDirPin1, LOW);
  digitalWrite(RightMotorDirPin2, HIGH);
  digitalWrite(LeftMotorDirPin1, HIGH);
  digitalWrite(LeftMotorDirPin2, LOW);
  set_Motorspeed('T');
}
