#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Phase.h"


Phase::Phase()
{
  _phase = 0;
  _isSwitchedLastTick = 1;
}

uint8_t Phase::advance()
{
  _isSwitchedLastTick = 1;
  return _phase = _phase + 1;
}


uint8_t Phase::getCurrentPhase()
{
  return _phase;
}

bool Phase::getWasSwitchedLastTick()
{
  return _isSwitchedLastTick;
}

void Phase::update()
{
  if (_isSwitchedLastTick = 1) {
    _isSwitchedLastTick = 0;
  }
}

