#ifndef Controller_h
#define Controller_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Score.h"
#include "Phase.h"


/**
 * Controller class is for writing data to the game model. (MVC paradigm)
 * Multiple classes need to do this (ex: Button, Radio) so a single interface was created for this purpose
 * to avoid situations where two separate classes changing game state cause issues.
 */
class Controller
{
  public:
    Controller(Score& score, Phase& phase);
    void advancePhase();
    void triggerButtonPress(bool buttonNumber);
    uint8_t getCurrentPhase();
  private:
    static Score& _score;
    static Phase& _phase;
};

#endif
