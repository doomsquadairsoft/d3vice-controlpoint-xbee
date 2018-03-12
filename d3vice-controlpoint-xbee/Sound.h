#ifndef Sound_h
#define Sound_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Phase.h"



class Sound
{
  public:
    Sound(uint8_t buzzerPin);
    void asyncMorse(char character);
    void asyncBeep(uint32_t duration);
    void asyncBeep();
    void update();
  private:
    uint8_t _buzzerPin;
    uint32_t _asyncBeepStartTime;
    uint32_t _asyncMorseStartTime;
    bool _isAsyncMorseComplete;
    bool _isAsyncBeepComplete;
    uint32_t _morseDitDuration;
    uint32_t _morseDahDuration;
    uint32_t _asyncBeepDuration;
};


#endif
