int getError() {
  lvalue = map(analogRead(LSENSOR), 0, 1023, 0, 3000);
  rvalue = map(analogRead(RSENSOR), 0, 1023, 0, 3000);
  return lvalue - rvalue;
}

// run robot wheels
void runMotors(int deltaL, int deltaR)
{
  int pulseL = (stopPulse + deltaL)*10;    //length of pulse in microseconds
  int pulseR = (stopPulse + deltaR)*10;
  leftWheel.writeMicroseconds(pulseL);
  rightWheel.writeMicroseconds(pulseR);
}

void turn(){
  if (lvalue > BLACK && rvalue > BLACK) {
    runMotors(Delta, Delta + offset);
    delay(80);

    runMotors(0, 0);
    delay(200);

    runMotors((TURN_SPEED), -(TURN_SPEED + offset + 2));
    delay(1150);

    runMotors((TURN_SPEED), (TURN_SPEED + offset));
    delay(100);

    runMotors(0, 0);
    delay(1000);
    if(laps == 2){
      stopNext = true;
      currentTime = millis();
    }

    error = getError();
    lastError = error;
    runMotors(0, 0);
    delay(1000);
  }

  else if (distance > 1300) {

    runMotors(0, 0);
    delay(100);

    runMotors(-(TURN_SPEED + 7), (TURN_SPEED + offset));
    delay(1260);

    if(getError() > 0){
      while(getError() > 0){
        runMotors((TURN_SPEED), -(TURN_SPEED + offset)/2);
      }
    }
    else if (getError() < 0){
      while(getError() < 0){
        runMotors(-(TURN_SPEED), (TURN_SPEED + offset)/2);
      }
    }
    laps = laps + 0.5;

    error = getError();
    lastError = error;
    runMotors(0, 0);
    delay(1000);

    if(fmod(laps, 1) == 0.5){ // pickup
      moveLift(0);
      delay(250);
      runMotors(-(Delta + 7)/2, -(Delta + offset + 7)/2);
      delay(2700);
      runMotors(0, 0);
      delay(250);
      moveLift(1);
    }
    else{
      delay(250);
      runMotors(-(Delta + 7)/2, -(Delta + offset + 7)/2);
      delay(2700);
      runMotors(0, 0);
      delay(250);
      moveBucket(0);
      delay(700);
      moveBucket(1);
      delay(250);
    }
  }
}

void moveLift (bool liftState){
  liftServo.attach(servoPinA);
  bucketServo.attach(servoPinB);

  if(liftState == 1){
    while (currentPosA >= Ahigh || currentPosB <= Bhigh) {
      if (currentPosA >= Ahigh) liftServo.write(currentPosA--);
      if (currentPosB <= Bhigh) bucketServo.write(currentPosB++);
      delay(20);
    }
  }
  else if(liftState == 0){
    while (currentPosA <= Alow || currentPosB >= Blow) {
      if (currentPosA <= Alow) liftServo.write(currentPosA++);
      if (currentPosB >= Blow) bucketServo.write(currentPosB--);
      delay(20);
    }
    liftServo.detach();
    bucketServo.detach();
  }
}

void moveBucket (bool bucketState){
  bucketServo.attach(servoPinB);

  if(bucketState == 0){
    for(; currentPosB <= Bdump; currentPosB++){
      bucketServo.write(currentPosB);
      delay(20);
    }
  }
  else if(bucketState == 1){
    for(; currentPosB >= Bhigh; currentPosB--){
      bucketServo.write(currentPosB);
      delay(20);
    }
  }
}

// Turn on a single LED and turn others off
void turnOnLED(int colour)
{
  digitalWrite(GRN, LOW);
  digitalWrite(YLW, LOW);
  digitalWrite(RED, LOW);
  digitalWrite(colour, HIGH);
}

// Light LED based on error direction
void led_direction(int error) {
  if (error > 0) {
    turnOnLED(RED);
  } else if (error < 0) {
    turnOnLED(GRN);
  } else {
    turnOnLED(YLW);
  }
}

//Toggle an LED on/off
void toggleLED(int colour){
  digitalWrite(GRN, LOW);
  digitalWrite(YLW, LOW);
  digitalWrite(RED, LOW);
  digitalWrite(colour, HIGH);
  delay(250);
  digitalWrite(colour, LOW);
  delay(250);
}
