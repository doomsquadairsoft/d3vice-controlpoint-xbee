#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Phase.h"


Phase::Phase()
{
  uint8_t _phase = 0;
}

uint8_t Phase::advance()
{
  return _phase = _phase + 1;
}


uint8_t Phase::getCurrentPhase()
{
  return _phase;
}

