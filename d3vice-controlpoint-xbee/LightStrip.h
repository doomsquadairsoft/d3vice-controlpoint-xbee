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
    LightStrip(Adafruit_NeoPixel& neoPixel);
    void update();
    void show(uint8_t phaseNumber);
  private:
    void _animatePhase4();
    uint8_t _pulsate(uint8_t breathState);
    Adafruit_NeoPixel& _neoPixelStrip;
    bool _isStarted;
    bool _isInhale;
    uint8_t _breathState;
    uint8_t _phase4AnimationState;
    float _sinIn;
};

#endif


