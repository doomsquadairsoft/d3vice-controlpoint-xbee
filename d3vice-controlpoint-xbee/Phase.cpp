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
Phase::Phase(uint8_t phase) : _phase(phase)
{
  _isSwitchedLastTick = 1; // starts as 1 so other classes know phase 0 just started
  _isPhaseAdvanceQueued = 0;
  _isPhaseGoToQueued = 0;
  _queuedGoToPhase = 0;
}

/**
 * advance
 * 
 * queues a phase advancement to happen during the next run of Phase::update()
 */
void Phase::advance()
{  
  _isPhaseAdvanceQueued = 1;
  return;
}


void Phase::goTo(uint8_t phase)
{
  if (phase < 2^8) {
    _isPhaseGoToQueued = 1;
    _queuedGoToPhase = phase;
  }
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
  if (_isSwitchedLastTick) {
    _isSwitchedLastTick = 0;
  }

  if (_isPhaseAdvanceQueued == 1) {
    _isPhaseAdvanceQueued = 0;
    _advance();
  }

  if (_isPhaseGoToQueued == 1) {
    _isPhaseGoToQueued = 0;
    _goTo(_queuedGoToPhase);
  }

}

/**
 * Phase::_advance()
 * 
 * Do the natural phase advancement.
 * Do not call Phase::_advance() function from other classes. Instead call Phase::advance()
 *
 * It is possible that phase advancments are not in order (ex: from 1 to 2)
 * It is possible that order may be more like (ex: 1 to 7)
 * This is because more phases may be added later on which fall outside natural integer progression.
 * In this function we can code any order we want, for cases in which advance() would be called from other classes.
 * It may make more sense to add a separate function for explicitly switching to a specific phase. (ex: Phase::goToPhase(20);)
 * 
 */
void Phase::_advance()
{
  _isSwitchedLastTick = 1;

  if (_phase == 6) {
    _phase = 0;
  }
  else {
    _phase += 1;
  }
  
  return;
}

/**
 * Phase::_goTo
 * 
 * go directly to the specified phase.
 * called internally by Phase::goTo
 * 
 * do not call this directly.
 */
void Phase::_goTo(uint8_t phase)
{
  _isSwitchedLastTick = 1;
  if (phase < 2^8) {
    _phase = phase;
  }
}

