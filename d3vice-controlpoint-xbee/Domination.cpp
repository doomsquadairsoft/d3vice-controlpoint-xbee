#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Domination.h"
#include "Event.h"
#include "Score.h"


/**
 * Domination Class
 * 
 * This class's job is to increment scores according to event inputs, and determine a winner
 */
Domination::Domination(Event& event, Score& score, Time& t) :
  _event(event), _score(score), _time(t)
{
  _event = event;
  _score = score;
  _time = t;
  _lastTickTime = 0;
}

void Domination::update() {
  // Calculate the duration since last tick
  unsigned long tickTime = _time.millis() - ;
  
  
  // If team 1's button is pressed,
  //   set team 1 as being in control of the point
  //
  // Else if team 2's button is pressed,
  //   set team 2 as being in control of the point
  
  if (_event.isPressed(0)) {
    _score.setControllingTeam(0);
  }
  else if (_event.isPressed(1)) {
    _score.setControllingTeam(1);
  }
  else {
    
    // If team 1 is in control of the point
    //   increment team 1's timer by the amount of time elapsed since last tick
    //
    // Else if team 2 is in control of the point
    //   increment team 2's timer by the amount of time elapsed since last tick

    if (_score.getControllingTeam() == 0) {
      _score.increment(0, tickTime
    }
  }
  
  //
  //
  // If team 1's timer has reached the time needed to win,
  //   team 1 wins
  //
  // Else if team 2's timer has reached the time needed to win,
  //   team 2 wins
  
  
  if (_event.isPressed) {

  }
}

