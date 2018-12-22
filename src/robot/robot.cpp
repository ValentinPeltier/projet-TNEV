#include "robot.h"

void Robot::init() {
  Serial.begin(9600);

  // Motors
  motorLeft.init(MOTOR_L_SPEED_PIN, MOTOR_L_CONTROL1_PIN, MOTOR_L_CONTROL2_PIN);
  motorRight.init(MOTOR_R_SPEED_PIN, MOTOR_R_CONTROL1_PIN, MOTOR_R_CONTROL2_PIN);

  // Servomotor
  servomotor.init(SERVOMOTOR_PIN);

  // Ultrasonic sensor
  ultrasonic.init(ULTRASONIC_TRIGGER_PIN, ULTRASONIC_ECHO_PIN);

  // Button
  button.init(BUTTON_PIN);
}

void Robot::loop() {
  initValues();
  waitForStart();
  searchFirstBottle();
  stop();
  bypassBottles();
  stop();
}

void Robot::initValues() {
  servoAngle = 0.0f;
  servoDirection = 1;
  servomotor.setValue(0);
}

void Robot::waitForStart() {
  IRrecv irrecv(INFRARED_PIN);
  irrecv.enableIRIn();
  decode_results results;

  bool start = false;

  while(!start) {
    start = button.click();

    // Remote control
    if (irrecv.decode(&results)) {
      // Forward
      if(results.value == 1282 || results.value == 3330 || results.value == 1333 || results.value == 3381) { // 2, top arrow
        motorLeft.set(forward, MOTOR_L_SPEED);
        motorRight.set(forward, MOTOR_R_SPEED);
      }
      // Backward
      else if(results.value == 1288 || results.value == 3336 || results.value == 1334 || results.value == 3382) { // 8, bottom arrow
        motorLeft.set(backward, MOTOR_L_SPEED);
        motorRight.set(backward, MOTOR_R_SPEED);
      }
      // Turn left
      else if(results.value == 1284 || results.value == 3332 || results.value == 1313 || results.value == 3361) { // 4, left arrow
        motorLeft.set(backward, MOTOR_L_SPEED);
        motorRight.set(forward, MOTOR_R_SPEED);
      }
      // Turn right
      else if(results.value == 1286 || results.value == 3334 || results.value == 1312 || results.value == 3360) { // 6, right arrow
        motorLeft.set(forward, MOTOR_L_SPEED);
        motorRight.set(backward, MOTOR_R_SPEED);
      }
      // Turn 90deg left
      else if(results.value == 1297 || results.value == 3345) { // - button
        turn(-90);
      }
      // Turn 90deg right
      else if(results.value == 1296 || results.value == 3344) { // + button
        turn(90);
      }
      // Start moving
      else if(results.value == 1325 || results.value == 3373) { // Power button
        start = true;
      }
      else {
        // Stop
        motorLeft.set(forward, 0);
        motorRight.set(forward, 0);
      }

      Serial.print("Remote : ");
      Serial.println(results.value);

      irrecv.resume();

      delay(120);
    }
    else {
      // Stop
      motorLeft.set(forward, 0);
      motorRight.set(forward, 0);
    }
  }
}

void Robot::searchFirstBottle() {
  goForward();

  float distance = ultrasonic.getDistance();

  // Go forward until the bottle is near enough
  while(distance > MAX_FIRST_BOTTLE_DISTANCE) {
    servoAngle += servoDirection * SERVOMOTOR_ROTATE_SPEED;

    if(abs(servoAngle) >= SERVOMOTOR_ROTATE_ANGLE) {
      servoDirection = -servoDirection;
      servoAngle = (servoAngle > 0 ? 1 : -1) * SERVOMOTOR_ROTATE_ANGLE;
    }

    servomotor.setValue(servoAngle);
    delay(10);
    distance = ultrasonic.getDistance();
  }
}

void Robot::bypassBottles() {
  servomotor.setValue(-90);
  turn(45);
  goForward();

  // Go forward until we see the bottle
  while(ultrasonic.getDistance() > MAX_BOTTLE_DISTANCE);

  // Continue until we pass the bottle (and a bit more)
  while(ultrasonic.getDistance() < MAX_BOTTLE_DISTANCE);
  delay(750);

  for(int i = 0; i < 4; i++) {
    servomotor.setValue((i % 2 ? -1 : 1) * 90);
    turn((i % 2 ? 1 : -1) * 90);
    goForward();
    
    // Go forward until we see the bottle
    while(ultrasonic.getDistance() > MAX_BOTTLE_DISTANCE);

    // Continue until we pass the bottle (and a bit more)
    while(ultrasonic.getDistance() < MAX_BOTTLE_DISTANCE);
    delay(750);
  }
}

void Robot::goForward() {
  motorLeft.set(forward, MOTOR_L_SPEED);
  motorRight.set(forward, MOTOR_R_SPEED);
}

void Robot::goBackward() {
  motorLeft.set(backward, MOTOR_L_SPEED);
  motorRight.set(backward, MOTOR_R_SPEED);
}

void Robot::stop() {
  motorLeft.setSpeed(0);
  motorRight.setSpeed(0);
}

void Robot::turn(int adeg) {
  // Convert adeg from deg to rad
  float turnAngle = DEG_TO_RAD * adeg;
  
  float d = abs(turnAngle) * TRACK / 100.0f; // Calculate distance (to meters)
  float t = d / SPEED * 1000; // Calculate time (to milliseconds)

  if(turnAngle > 0) {
    motorLeft.set(forward, MOTOR_L_SPEED);
    motorRight.set(backward, MOTOR_R_SPEED);
    t /= MOTOR_ROTATE_SHIFT;
  }
  else {
    motorLeft.set(backward, MOTOR_L_SPEED);
    motorRight.set(forward, MOTOR_R_SPEED);
    t *= MOTOR_ROTATE_SHIFT;
  }

  delay(t);

  motorLeft.set(forward, 0);
  motorRight.set(forward, 0);
}