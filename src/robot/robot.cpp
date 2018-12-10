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
  // Init
  servoAngle = 0.0f;
  servoDirection = 1;
  servomotor.setValue(0);

  IRrecv irrecv(INFRARED_PIN);
  irrecv.enableIRIn();
  decode_results results;

  bool move = false;

  // Wait until move button is pressed
  while(!move) {
    move = moveButton.click();

    // Remote control
    if (irrecv.decode(&results)) {
      Serial.print("Remote : ");
      Serial.println(results.value);

      // Forward
      if(results.value == 1282 || results.value == 3330 || results.value == 1333 || results.value == 3381) {
        motorLeft.set(forward, 255);
        motorRight.set(forward, 255);
      }
      // Backward
      else if(results.value == 1288 || results.value == 3336 || results.value == 1334 || results.value == 3382) {
        motorLeft.set(backward, 255);
        motorRight.set(backward, 255);
      }
      // Turn left
      else if(results.value == 1284 || results.value == 3332 || results.value == 1313 || results.value == 3361) {
        motorLeft.set(backward, 255);
        motorRight.set(forward, 255);
      }
      // Turn right
      else if(results.value == 1286 || results.value == 3334 || results.value == 1312 || results.value == 3360) {
        motorLeft.set(forward, 255);
        motorRight.set(backward, 255);
      }
      // Turn 90deg left
      else if(results.value == 1297 || results.value == 3345) {
        turn(-90);
      }
      // Turn 90deg right
      else if(results.value == 1296 || results.value == 3344) {
        turn(90);
      }
      // Start moving
      else if(results.value == 1325 || results.value == 3373) {
        move = true;
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

  // Go forward until distance < 25cm
  while(distance > 25.0f) {
    delay(10);

    servoAngle += servoDirection * 2.5f;

    if(abs(servoAngle) > 40.0f) {
      servoDirection = -servoDirection;
    }

    servomotor.setValue(servoAngle);

    distance = ultrasonic.getDistance();
  }
  
  motorLeft.set(forward, 0);
  motorRight.set(forward, 0);

  // Turn around the bottle

  servomotor.setValue(-90);
  turn(45);
  motorLeft.set(forward, 255);
  motorRight.set(forward, 255);
  
  // Go forward until we see the bottle
  while(ultrasonic.getDistance() > 30.0f);

  // Continue until we pass the bottle (and a bit more)
  while(ultrasonic.getDistance() < 30.0f);
  delay(750);

  for(int i = 0; i < 4; i++) {
    servomotor.setValue((i%2?-1:1)*90);
    turn((i%2?1:-1)*90);
    motorLeft.set(forward, 255);
    motorRight.set(forward, 255);
    
    // Go forward until we see the bottle
    while(ultrasonic.getDistance() > 30.0f);

    // Continue until we pass the bottle (and a bit more)
    while(ultrasonic.getDistance() < 30.0f);
    delay(750);
  }

  // Stop
  motorLeft.set(forward, 0);
  motorRight.set(forward, 0);
}

void Robot::turn(int adeg) {
  // Convert adeg from deg to rad
  float turnAngle = PI * adeg / 180.0f;
  
  float l = abs(turnAngle) * TRACK / 100.0f; // (Convert to m)
  float t = l / SPEED * 1000; // (Convert to millis)

  if(turnAngle > 0) {
    motorLeft.set(forward, 255);
    motorRight.set(backward, 255);
  }
  else {
    motorLeft.set(backward, 255);
    motorRight.set(forward, 255);
  }

  delay(t);

  motorLeft.set(forward, 0);
  motorRight.set(forward, 0);
}