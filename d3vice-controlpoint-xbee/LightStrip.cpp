#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "LightStrip.h"
#include "Score.h"


LightStrip::LightStrip(uint8_t lightStripPin, Score& score)
{
  _lightStripPin = lightStripPin;
}

void LightStrip::update()
{
  
}
