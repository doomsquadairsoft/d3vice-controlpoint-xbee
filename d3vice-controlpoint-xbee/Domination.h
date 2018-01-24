#ifndef Domination_h
#define Domination_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Score.h"

class Domination
{
  public:
    Domination(Score& score);
    void update();
  private:
    static Score& _score;
};

#endif
