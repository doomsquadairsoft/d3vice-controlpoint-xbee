#ifndef Radio_h
#define Radio_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include <XBee.h>
#include "Phase.h"
#include "Controller.h"


class Radio
{
  public:
    Radio(XBee& xbee, Phase* phase, Controller* controller);
    void update();
  private:
    XBee& _XBee;
    Controller* _controller;
    Phase* _phase;
};

#endif
