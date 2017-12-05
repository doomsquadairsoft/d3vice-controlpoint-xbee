#ifndef LightStrip_h
#define LightStrip_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include <Adafruit_NeoPixel.h>
#include "Score.h"


class LightStrip
{
  public:
    LightStrip(Adafruit_NeoPixel& neoPixel, Score& score);
    void update();
  private:
    void _animate();
    Adafruit_NeoPixel& _neoPixelStrip;
    Score& _score;
    bool _isStarted;
    bool _isInhale;
    uint8_t _breathState;
    uint8_t _animationState;
    float _sinIn;
};

#endif


