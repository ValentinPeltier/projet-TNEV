#include "robot.h"

void Robot::init() {
  Serial.begin(9600);

  // Motors
  motorLeft.init(MOTOR_L_SPEED_PIN, MOTOR_L_CONTROL1_PIN, MOTOR_L_CONTROL2_PIN);
  motorRight.init(MOTOR_R_SPEED_PIN, MOTOR_R_CONTROL1_PIN, MOTOR_R_CONTROL2_PIN);

  motorLeft.set(forward, 0);
  motorRight.set(forward, 0);

  // Servomotor
  servomotor.init(SERVOMOTOR_PIN);

  // Ultrasonic sensor
  ultrasonic.init(ULTRASONIC_TRIGGER_PIN, ULTRASONIC_ECHO_PIN);

  // Buttons
  moveButton.init(BUTTON_MOVE_PIN);
  shootButton.init(BUTTON_SHOOT_PIN);
}

void Robot::loop() {
  const int RECV_PIN = 2;
  IRrecv irrecv(RECV_PIN);
  decode_results results;

  irrecv.enableIRIn();
  irrecv.blink13(true);

  // Wait until move button is pressed
  while(!moveButton.click()) {
    // Remote
    if (irrecv.decode(&results)) {
      Serial.println(results.value);

      if(results.value == 1282 || results.value == 3330 || results.value == 1333 || results.value == 3381) {
        // Go forward
        motorLeft.set(forward, 255);
        motorRight.set(forward, 255);
      }
      else if(results.value == 1284 || results.value == 3332 || results.value == 1313 || results.value == 3361) {
        // Turn left
        motorLeft.set(backward, 255);
        motorRight.set(forward, 255);
      }
      else if(results.value == 1286 || results.value == 3334 || results.value == 1312 || results.value == 3360) {
        // Turn right
        motorLeft.set(forward, 255);
        motorRight.set(backward, 255);
      }
      else if(results.value == 1288 || results.value == 3336 || results.value == 1334 || results.value == 3382) {
        // Go backward
        motorLeft.set(backward, 255);
        motorRight.set(backward, 255);
      }
      else {
        // Stop
        motorLeft.set(forward, 0);
        motorRight.set(forward, 0);
      }

      irrecv.resume();

      delay(100);
    }
    else {
      // Stop
      motorLeft.set(forward, 0);
      motorRight.set(forward, 0);
    }
  }

  // Go forward
  motorLeft.set(forward, 255);
  motorRight.set(forward, 255);

  float distance = ultrasonic.getDistance();

  // Go forward until distance < 30cm
  while(distance > 20.0f) {
    delay(10);

    servoAngle += servoDirection * 2.0f;

    if(abs(servoAngle) > 30.0f) {
      servoDirection = -servoDirection;
    }

    servomotor.setValue(servoAngle);

    distance = ultrasonic.getDistance();
  }
  
  motorLeft.set(forward, 0);
  motorRight.set(forward, 0);

  // Start the curve around the bottle
  
  turn(45);
  while(actionManager.getCount() > 0) {
    actionManager.update();
  }

  motorLeft.set(forward, 255);
  motorRight.set(forward, 255);

  delay(2000);

  turn(-90);
  while(actionManager.getCount() > 0) {
    actionManager.update();
  }

  motorLeft.set(forward, 255);
  motorRight.set(forward, 255);

  delay(2000);

  turn(45);
  
  while(actionManager.getCount() > 0) {
    actionManager.update();
  }

  motorLeft.set(forward, 0);
  motorRight.set(forward, 0);

  servoAngle = 0.0f;
  servoDirection = 1;
}

void Robot::turn(int adeg) {
  // Convert adeg from deg to rad
  turnAngle = PI * adeg / 180.0f;
  
  float l = abs(turnAngle) * TRACK / 100.0f; // (Convert to m)
  float t = l / (SPEED * 1.18125f) * 1000; // (Convert to millis)

  actionManager.addAction(this, &Robot::turnRoutine, t);
}

void Robot::turnRoutine(float p) {
  if(turnAngle == 0) {
    return;
  }

  if(turnAngle > 0) {
    motorLeft.set(forward, 255);
    motorRight.set(backward, 255);
  }
  else {
    motorLeft.set(backward, 255);
    motorRight.set(forward, 255);
  }
}