#include "robot.h"

/*
void Robot::foo() {
  Serial.println("I'm in foo");
}

actionManager.addAction(this, &Robot::foo, 1000);

while(actionManager.getCount() > 0) {
  actionManager.update();
}
*/

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
  servomotor.setValue(0);

  // Wait until move button is pressed
  while(!moveButton.click());

  // Go forward
  motorLeft.set(forward, 255);
  motorRight.set(forward, 255);

  float distance = ultrasonic.getDistance();

  while(distance > 20.0f) {
    delay(10);
    
    servoAngle += servoDirection * 0.5f;

    if(abs(servoAngle) > 30.0f) {
      servoDirection = -servoDirection;
    }

    servomotor.setValue(servoAngle);

    distance = ultrasonic.getDistance();
  }

  motorLeft.set(forward, 0);
  motorRight.set(forward, 0);
}

void Robot::turn(int adeg) {
  // Convert adeg from deg to rad
  float angle = PI * adeg / 180.0f;
  
  float l = abs(angle) * TRACK / 100.0f; // (Convert to m)
  float t = l / (SPEED * 1.5f) * 1000; // (Convert to millis)
    
  if(angle > 0) {
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