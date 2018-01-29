#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "ButtonManager.h"
#include "Button.h"
#include "Phase.h"



ButtonManager::ButtonManager(Button& team0Button, Button& team1Button, Phase* phase)
: _team0Button(team0Button), _team1Button(team1Button)
{
  _team0Button = team0Button;
  _team1Button = team1Button;
  _phase = phase;
}


void ButtonManager::update()
{

  /**
   * Phase 2-- Programming > Game mode.
   *   The phase where the type of game is chosen.
   *   Red button cycle forward through game modes
   *   Green button cycles backward throu game mode
   *   Both buttons held together advance to next phase
   */
  if (_phase->getCurrentPhase() == 2) {
    // handle both buttons pressed simultaneously
    if (_team0Button.getState() == 1 && _team1Button.getState() == 1) {
      _phase->advance();
    }
  }
}

