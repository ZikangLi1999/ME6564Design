// Linear Actuator
// Course: Complex System
// Author: Zikang Li
// Date: 2023-4-18

#include <Arduino.h>
#include "pushrod.h"

PushRod::PushRod(int pinP, int pinN, float maxpos, float v)
{
  PinPositive = pinP;
  PinNegative = pinN;
  MaxPosition = maxpos;   // cm
  Velocity = v;           // cm/s

  pinMode(PinPositive, OUTPUT);
  pinMode(PinNegative, OUTPUT);

  digitalWrite(PinPositive, LOW);
  digitalWrite(PinNegative, LOW);
}

bool PushRod::RunForTime(int dir, float duration)
{
  // Check value
  float allowedDuration;
  if (dir > 0) allowedDuration = 1e3 * (MaxPosition - CurrentPosition) / Velocity;
  if (dir < 0) allowedDuration = 1e3 * CurrentPosition / Velocity;
  duration = (duration < allowedDuration) ? duration : allowedDuration;

  // Run
  float startTime, endTime, position;
  startTime = micros();

  if (dir > 0)
  {
    digitalWrite(PinPositive, HIGH);
    digitalWrite(PinNegative, LOW);
  }
  if (dir < 0)
  {
    digitalWrite(PinPositive, LOW);
    digitalWrite(PinNegative, HIGH);
  }
  delay(duration);                        // Keep moving

  digitalWrite(PinPositive, LOW);         // Stop
  digitalWrite(PinNegative, LOW);

  endTime = micros();
  CurrentPosition += dir * Velocity * (endTime - startTime) * 1e-3;    // m/s * ms * 1e-3

  return ((endTime - startTime) == duration);
}

bool PushRod::RunToPosition(float trgPosition)
{
  // Check value
  if (trgPosition < 0.) trgPosition = 0.;
  if (trgPosition > MaxPosition) trgPosition = MaxPosition;

  // Run
  int dir;
  float startTime, endTime;

  startTime = micros();
  if (CurrentPosition > trgPosition)
  {
    dir = -1;
    int duration = (int) ((CurrentPosition - trgPosition) / Velocity * 1e3);
    digitalWrite(PinPositive, LOW);
    digitalWrite(PinNegative, HIGH);
    delay(duration);
  }
  if (CurrentPosition < trgPosition)
  {
    dir = 1;
    int duration = (int) ((trgPosition - CurrentPosition) / Velocity * 1e3);
    digitalWrite(PinPositive, HIGH);
    digitalWrite(PinNegative, LOW);
    delay(duration);
  }
  CurrentPosition = trgPosition;

  // while (1)
  // {
  //   endTime = micros();
  //   CurrentPosition += dir * Velocity * (endTime - startTime) * 1e-3;

  //   if (((dir > 0) && (CurrentPosition >= trgPosition)) | ((dir < 0) && (CurrentPosition <= trgPosition)))
  //   {
  //     digitalWrite(PinPositive, LOW);
  //     digitalWrite(PinNegative, LOW);
  //     break;
  //   }
  // }
  return (CurrentPosition == trgPosition);
}

PushRod::~PushRod()
{
  RunToPosition(0.);
}
