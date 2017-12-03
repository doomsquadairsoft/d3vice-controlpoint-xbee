#ifndef LightStrip_h
#define LightStrip_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Score.h"



class LightStrip
{
  public:
    LightStrip(uint8_t lightStripPin, Score& score);
    void update();
  private:
    uint8_t _lightStripPin;
    Score& _score;
};

#endif


