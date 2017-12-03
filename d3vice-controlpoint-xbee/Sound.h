#ifndef Sound_h
#define Sound_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Score.h"

class Sound
{
  public:
    Sound(uint32_t duration, uint8_t buzzerPin, Score& score);
    void update();
  private:
    uint32_t _duration;
    uint8_t _buzzerPin;
    Score& _score;
};


#endif
