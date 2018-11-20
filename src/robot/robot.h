#ifndef ROBOT_H
#define ROBOT_H

#include "consts.h"
#include "button.h"
#include "motor.h"
#include "servomotor.h"
#include "ultrasonic.h"
#include "actionManager.h"

#define X 2300
#define Y 2500

class Robot {
public:
	void init();
	void loop();

private:
	void turn(int);
	void updateButtonState();
	bool buttonStatePressed();

	// -------

	float servoAngle = 0.0f;
	int servoDirection = 1;

	Button moveButton, shootButton;
	Motor motorLeft, motorRight;
	Servomotor servomotor;
	Ultrasonic ultrasonic;

	ActionManager actionManager;
};

#endif