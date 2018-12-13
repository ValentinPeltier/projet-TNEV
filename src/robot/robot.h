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
	void initValues();
	void waitForStart();
	void searchFirstBottle();
	void bypassBottles();
	void goForward();
	void goBackward();
	void stop();
	void turn(int);

	// -------
	
	float servoAngle;
	int servoDirection;

	Button button;
	Motor motorLeft, motorRight;
	Servomotor servomotor;
	Ultrasonic ultrasonic;
};

#endif