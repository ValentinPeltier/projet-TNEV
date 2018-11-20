#ifndef ACTION_MANAGER_H
#define ACTION_MANAGER_H

#include <Arduino.h>

class Robot;

typedef void (Robot::*RobotActionFn)(float);

struct Action {
	Robot *robot;
	RobotActionFn func;
	long duration;
	long startTime;
};

class ActionManager {
public:
	void addAction(Robot*, RobotActionFn, long);
	void update();
	int getCount();
	
private:
	Action actions[100];
	int actionCount = 0;
};

#endif
