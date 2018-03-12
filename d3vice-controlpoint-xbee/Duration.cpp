#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Duration.h"




Duration::Duration(uint8_t initialDuration = 10)
{
  _duration = initialDuration;
}

void Duration::set(uint8_t duration)
{
  _duration = duration;  
}

void Duration::increment()
{
  if (_duration == 255) {
    _duration = 0;
  }
  else {
    _duration += 1;
  }
}

void Duration::decrement()
{
  if (_duration == 0) {
    _duration = 255;
  }
  else {
    _duration -= 1;
  }
}

uint8_t Duration::get()
{
  return _duration;
}

