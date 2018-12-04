#include "ultrasonic.h"

void Ultrasonic::init(int triggerPin, int echoPin) {
  this->triggerPin = triggerPin;
  this->echoPin = echoPin;

  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

float Ultrasonic::getDistance() {
  // If last call is more than 60ms ago
  if(millis() > lastTime + 60) {
    // Calculate distance
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);

    long duration = pulseIn(echoPin, HIGH);

    if(duration <= 0) {
      return lastDistance;
    }

    float distance = duration / 2.0f / 29.1f;

    if(distance >= 400.0f) {
      distance = 400.0f;
    }

    lastTime = millis();
    lastDistance = distance;

    return distance;
  }
  else {
    // Return last value
    return lastDistance;
  }
}