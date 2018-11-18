#include "actionManager.h"

void ActionManager::addAction(Robot* robot, RobotActionFn func, long int delay) {
  actions[actionCount++] = { robot, func, millis() + delay };
}

void ActionManager::update() {
  int i = 0;
  while(i < actionCount) {
    if(actions[i].time <= millis()) {
      ((actions[i].robot)->*(actions[i].func))();

      // Shift actions
      for(int j = i; j < actionCount; j++) {
        actions[j-1] = actions[j];
      }

      actionCount--;
    }
    else {
      i++;
    }
  }
}

int ActionManager::getCount() {
  return actionCount;
}