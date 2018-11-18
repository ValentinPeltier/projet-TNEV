#ifndef ACTION_MANAGER_H
#define ACTION_MANAGER_H

#include <Arduino.h>

class Robot;

typedef void (Robot::*RobotActionFn)();

struct Action {
	Robot *robot;
	RobotActionFn func;
	long int time;
};

class ActionManager {
public:
	void addAction(Robot*, RobotActionFn, long int);
	void update();
	int getCount();
	
private:
	Action actions[100];
	int actionCount = 0;
};

#endif
