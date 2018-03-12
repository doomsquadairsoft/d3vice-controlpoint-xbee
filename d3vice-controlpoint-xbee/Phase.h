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
    Phase(uint8_t phase);
    void advance();
    void goTo(uint8_t phase);
    uint8_t getCurrentPhase();
    bool getWasSwitchedLastTick();
    void update();
  private:
    int _phase;
    void _advance();
    void _goTo(uint8_t phase);
    bool _isSwitchedLastTick;
    bool _isPhaseAdvanceQueued;
    bool _isPhaseGoToQueued;
    uint8_t _queuedGoToPhase;
};

#endif
