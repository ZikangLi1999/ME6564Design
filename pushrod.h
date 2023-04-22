// Linear Actuator
// Course: Complex System
// Author: Zikang Li
// Date: 2023-4-18

#ifndef _PUSH_ROD_
#define _PUSH_ROD_

class PushRod
{
  private:
    unsigned int PinPositive, PinNegative;
    float MaxPosition, Velocity, CurrentPosition=0., LastActionTime=0.;
  
  public:
    PushRod(int pinP, int pinN, float maxpos, float v);
    bool RunForTime(int dir, float duration);
    bool RunToPosition(float trgPosition);
    ~PushRod();
};

#endif
