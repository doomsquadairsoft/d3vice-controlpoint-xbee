#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Radio.h"
#include <XBee.h>
#include "Phase.h"


Radio::Radio(XBee& xbee) :
  _XBee(xbee) {}

void Radio::update()
{
  // If the d3vice is in HELLO phase, Radio module is up!
  // Query the XBee network to see if a game is in progress.
  // If a game is in progress, syncronize with it's state.


  // @todo The radio module will need to syncronize with the XBee network during other phases.
  //
  
}


