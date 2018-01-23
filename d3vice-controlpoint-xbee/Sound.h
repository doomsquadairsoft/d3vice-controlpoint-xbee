#ifndef Sound_h
#define Sound_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Score.h"
#include "Phase.h"

class Sound
{
  public:
    Sound(uint32_t duration, uint8_t buzzerPin, Score& score, Phase& phase);
    void asyncMorse(char character);
    void asyncBeep(uint32_t duration);
    void update();
  private:
    uint32_t _duration;
    uint8_t _buzzerPin;
    uint32_t _asyncBeepStartTime;
    uint32_t _asyncMorseStartTime;
    Score& _score;
    Phase& _phase;
    bool _isAsyncMorseComplete;
    bool _isAsyncBeepComplete;
    uint32_t _morseDitDuration;
    uint32_t _morseDahDuration;
};


#endif
