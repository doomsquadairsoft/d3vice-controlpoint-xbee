#ifndef Game_h
#define Game_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Event.h"
#include "Score.h"

class Domination
{
  public:
    Domination(Event& event, Score& score);
    void update();
  private:
    Score& _score;
    Event& _event;
};

#endif
