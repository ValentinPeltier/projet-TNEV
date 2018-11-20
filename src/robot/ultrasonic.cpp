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
    digitalWrite(ULTRASONIC_TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(ULTRASONIC_TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(ULTRASONIC_TRIGGER_PIN, LOW);

    unsigned long duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);

    float distance = duration / 2.0f / 29.1f;

    if(distance < 0.0f) {
      distance = 0.0f;
    }
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