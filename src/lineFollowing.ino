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
Servo liftServo;
Servo bucketServo;
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
const float Kp = 0.0032;
const float Kd = 0.00001;

int lastError = 0;
float laps = 0;
bool stopNext = false;
long currentTime = 0;

int Alow = 160;    // initial angle
int Blow = 86;

int Ahigh = 115;     // highest angle
int Bhigh = 92;

int Bdump = 135;

int currentPosA = Alow;
int currentPosB = Blow;

unsigned long lastTurn = millis();
int backOffset = 8;


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

  moveLift(1);
  moveBucket(1);

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
  turn();

  error = getError();

  float derivative = error - lastError;
  float correction = (Kp * error) + (Kd * derivative);

  lastError = error;

  deltaL = (int)(Delta + correction);
  deltaR = (int)(Delta - correction);

  //Motor Speed Cap
  deltaL = constrain(deltaL, 0, MAX_DELTA);
  deltaR = constrain(deltaR, 0, MAX_DELTA);


  led_direction(error);
  if (laps < 2){
    runMotors(deltaL, deltaR);
  }
  else runMotors(0,0);

  if(millis() - currentTime > 10000 && stopNext == true){
    while(true){
      runMotors(0, 0);
    }
  }
}
