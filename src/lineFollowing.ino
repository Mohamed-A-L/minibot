#include <Servo.h>

#define MAX_DELTA 15
#define BLACK 2200
#define WHITE 600
#define TURN_SPEED 8

#define RED 10
#define GRN 9
#define YLW 5

Servo leftWheel;
Servo rightWheel;
int servoPinB = 12;     // Bucket servomotor #2 pin
int servoPinA = 11;     // Bucket servomotor #1 pin

int error;
int deltaL;
int deltaR;

const int BUTTON = 7;
const int MOTOR_R = 3;
const int MOTOR_L = 4;

const int LSENSOR = A1;
const int RSENSOR = A2;
const int SHARP = A3;

const int stopPulse = 148;

int lvalue = 0;  //left sensor value
int rvalue = 0;  //right sensor value
int distance = 0; //SHARP sensor value

int Delta = 8;
const int offset = 1;

//plugged in values
const float Kp = 0.0035;
const float Kd = 0.0000055;

int lastError = 0;
bool dir = 0;
float laps = 0;
bool stopNext = false;
long currentTime = 0;


void setup(){
  pinMode(GRN, OUTPUT);
  pinMode(YLW, OUTPUT);
  pinMode(RED, OUTPUT);

  pinMode(BUTTON, INPUT_PULLUP);

  pinMode(LSENSOR, INPUT);
  pinMode(RSENSOR, INPUT);
  pinMode(SHARP, INPUT);

  leftWheel.attach(MOTOR_L);
  rightWheel.attach(MOTOR_R);
  Serial.begin(9600);

  // myServoA.attach(servoPinA);
  // myServoA.write(posA);
  // myServoB.attach(servoPinB);
  // myServoB.write(Blow);
  // myServoA.detach(servoPinA);
  // myServoB.detach(servoPinB);

  while(digitalRead(BUTTON) == LOW){
    digitalWrite(GRN, HIGH);
    delay(125);
    digitalWrite(GRN, LOW);
    delay(125);
    runMotors(0,0);
  }
}

void loop() {
  distance = map(analogRead(SHARP), 0, 1023, 0, 3300);
  turn(dir);

  error = getError();

  float derivative = error - lastError;
  float correction = (Kp * error) + (Kd * derivative);

  lastError = error;

  deltaL = Delta + (int)correction;
  deltaR = Delta - (int)correction;

  //Motor Speed Cap
  deltaL = constrain(deltaL, 0, MAX_DELTA);
  deltaR = constrain(deltaR, 0, MAX_DELTA);


  led_direction(error);
  runMotors(deltaL, deltaR);

  if(millis() - currentTime > 10000 && stopNext == true){
    while(true){
      runMotors(0, 0);
    }
  }
}
