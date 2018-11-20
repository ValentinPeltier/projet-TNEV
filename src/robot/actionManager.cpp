#include "actionManager.h"

void ActionManager::addAction(Robot* robot, RobotActionFn func, long duration) {
  actions[actionCount++] = { robot, func, duration, millis() };
}

void ActionManager::update() {
  int i = 0;

  // For each action
  while(i < actionCount) {
    // If action should be executed
    if(millis() <= actions[i].startTime + actions[i].duration) {
      // Calculate action progression
      float progression = ((float)(millis() - actions[i].startTime)) / ((float)actions[i].duration);

      // Call action
      ((actions[i].robot)->*(actions[i].func))(progression);
    }
    else {
      // Remove action by shifting others
      for(int j = i; j < actionCount; j++) {
        actions[j-1] = actions[j];
      }

      actionCount--;
    }
  }
}

int ActionManager::getCount() {
  return actionCount;
}