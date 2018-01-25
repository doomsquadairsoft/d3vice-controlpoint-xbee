#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Phase.h"

/**
 * PHASES
 *   * Phases are a combination of preparatory programming steps, and game modes.
 *   * Phases determine how the discreet classes (Button.cpp, Domination.cpp, LED.cpp)
 *     react to changes in game state.
 *   
 *   Phase 0-- Test phase
 *     A test sequence runs, lighting up every LED and testing the buzzer.
 *     Allows user to verify that all LED & sound hardware is functioning properly. 
 *      
 *   Phase 1-- Hello phase. 
 *     D3VICE network syncronization. D3VICE finds an existing game on the XBee network
 *     if one exists. If no game is found, D3VICE switches to standalone mode and enters programming phase.
 *   
 *   Phase 2-- Programming > Game mode
 *     D3VICE buttons act as input for choosing the game mode
 *     
 *   Phase 3-- Programming > Domination > duration
 *     D3VICE buttons act as input for choosing the total accumulated time a team needs to control the point to win
 *     
 *   Phase 4-- Domination > Run
 *   Phase 5-- Domination > Pause
 *   Phase 6-- Domination > Win
 *   
 */
Phase::Phase()
{
  _phase = 0;
  _isSwitchedLastTick = 1;
}

uint8_t Phase::advance()
{
  _isSwitchedLastTick = 1;

  if (_phase == 6) {
    _phase = 0;
  }
  else {
    _phase += 1;
  }
  
  return _phase;
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

