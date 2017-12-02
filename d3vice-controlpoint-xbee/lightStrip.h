#ifndef LightStrip_h
#define LightStrip_h

#include "Arduino.h"

class LightStrip
{
  public:
    LightStrip(uint8_t lightstripPin);
    void update();
  private:
    bool _lightstripPin;
};

#endif
