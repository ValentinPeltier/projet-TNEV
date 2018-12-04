#ifndef ROBOT_H
#define ROBOT_H

#include "consts.h"
#include "button.h"
#include "motor.h"
#include "servomotor.h"
#include "ultrasonic.h"

#include <IRremote.h>

class Robot {
public:
	void init();
	void loop();

private:
	void turn(int);
	void updateButtonState();
	bool buttonStatePressed();

	// -------

	float turnAngle;
	float servoAngle;
	int servoDirection;

	Button moveButton, shootButton;
	Motor motorLeft, motorRight;
	Servomotor servomotor;
	Ultrasonic ultrasonic;
};

#endif