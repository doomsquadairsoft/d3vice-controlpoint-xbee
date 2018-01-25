#ifndef Phase_h
#define Phase_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif


/** 
 *  For a good time https://www.youtube.com/user/thirdphaseofmoon/
 */
class Phase
{
  public:
    Phase();
    uint8_t advance();
    uint8_t getCurrentPhase();
    bool getWasSwitchedLastTick();
    void update();
  private:
    uint8_t _phase;
    bool _isSwitchedLastTick;
};

#endif
