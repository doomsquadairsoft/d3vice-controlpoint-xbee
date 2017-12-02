#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "LightStrip.h"


LightStrip::LightStrip(uint8_t lightStripPin)
{
  _lightStripPin = lightStripPin;
}

void LightStrip::update()
{
  
}
