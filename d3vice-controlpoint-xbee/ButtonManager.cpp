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
    if (_team0Button.getState() == 2 && _team1Button.getState() == 2) {
      _phase->advance();
    }
  }


  /**
   * Phase 3-- Programming > Domination > duration.
   *   The phase when the user chooses the total cumulative time a team needs to control the point to win.
   *   Green button increments the time by 1 minute (up to a maximum of 595 hours)
   *   Red button decrements the time by 1 minute (down to a minimum of 1 second)
   *   Holding both buttons saves the selection and moves to the next phase (game running)
   */
   if (_phase->getCurrentPhase() == 2) {
     if (_team0Button.getState() == 2 && _team1Button.getState() == 2) {
       _phase->advance();
     }
   }


  
}

