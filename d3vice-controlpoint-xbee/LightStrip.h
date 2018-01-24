#ifndef LightStrip_h
#define LightStrip_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include <Adafruit_NeoPixel.h>
#include "Score.h"
#include "Phase.h"


class LightStrip
{
  public:
    LightStrip(Adafruit_NeoPixel& neoPixel, Score& score, Phase& phase);
    void update();
  private:
    void _animate();
    uint8_t _pulsate(uint8_t breathState);
    Adafruit_NeoPixel& _neoPixelStrip;
    static Score& _score;
    static Phase& _phase;
    bool _isStarted;
    bool _isInhale;
    uint8_t _breathState;
    uint8_t _animationState;
    float _sinIn;
};

#endif


