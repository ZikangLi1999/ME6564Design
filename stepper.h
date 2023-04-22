// Stepper Driver
// Course: Complex System
// Author: Zikang Li
// Date: 2023-3-30

// Note
//   - Set A4988 to 16 divied mode (MS1=1, MS2=1, MS3=1), which provides the slowest speed
//   - 200 * 16 steps == 1 round

#ifndef _STEPPER_
#define _STEPPER_

class Stepper
{
  private:
    unsigned int DirPin, PulPin, EnablePin;
    unsigned long StepTime = 3000, LastStepStart = 0;    // StepTime: the time duration (ms) that a step costs
    //                                                   // LastStepStart: the time that the last step started
    long CurrentStep = 0, TargetStep = 0;

    bool IsRunToTarget();
    bool CanRun();
    void runStep(bool dir);
  
  public:
    Stepper(int dir, int pul, int en);
    bool RunToStep(long target);
    bool RunToAngle(float trgAngle);
    ~Stepper();
};

#endif
