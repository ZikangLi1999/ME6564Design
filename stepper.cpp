// Stepper Driver
// Course: Complex System
// Author: Zikang Li
// Date: 2023-3-30

// Note
//   - Set A4988 to 16 divied mode (MS1=1, MS2=1, MS3=1), which provides the slowest speed
//   - 200 * 16 steps == 1 round

#include "Arduino.h"
#include "stepper.h"

Stepper::Stepper(int dir, int pul, int en)
{
    DirPin = dir;
    PulPin = pul;
    EnablePin = en;

    pinMode(DirPin, OUTPUT);
    pinMode(PulPin, OUTPUT);
    pinMode(EnablePin, OUTPUT);

    digitalWrite(DirPin, LOW);
    digitalWrite(PulPin, LOW);
    digitalWrite(EnablePin, LOW);
}

bool Stepper::RunToStep(long target)
{
    TargetStep = target;
    while (1) {
    if ((IsRunToTarget() == false) && (CanRun()))
    {
        runStep(CurrentStep < TargetStep);
    }
    if (IsRunToTarget()) break;
    }

    return IsRunToTarget();
}

bool Stepper::RunToAngle(float trgAngle)
{
    long trgStep;
    trgStep = (long) (trgAngle / 360 * 200 * 16);
    return RunToStep(trgStep);
}

bool Stepper::IsRunToTarget()
{
    return TargetStep == CurrentStep;
}

bool Stepper::CanRun()
{
    // The next step can be run a StepTime after the last step started
    return micros() - LastStepStart >= StepTime;
}

void Stepper::runStep(bool dir)
{
    LastStepStart = micros();
    digitalWrite(DirPin, dir);
    digitalWrite(PulPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(PulPin, LOW);
    CurrentStep += (dir ? 1 : -1);
}

Stepper::~Stepper()
{
  RunToAngle(0.);
}
