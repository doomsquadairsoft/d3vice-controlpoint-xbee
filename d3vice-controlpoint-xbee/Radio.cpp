#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Radio.h"
#include <XBee.h>
#include "Controller.h"
#include "Phase.h"


Radio::Radio(XBee& xbee, Phase* phase, Controller* controller) :
  _XBee(xbee)
{
  _XBee = xbee;
  _phase = phase;
  _controller = controller;
}

void Radio::update()
{
  // If the d3vice is in HELLO phase, Radio module is up!
  // Query the XBee network to see if a game is in progress.
  // If a game is in progress, syncronize with it's state.
  if (_phase->getCurrentPhase() == 1) {
    // @todo

    // just skip to the next phase because XBee functionality is not implemented yet.
    _controller->advancePhase();
  }

  // @todo The radio module will need to syncronize with the XBee network during other phases.
  //
  
}


