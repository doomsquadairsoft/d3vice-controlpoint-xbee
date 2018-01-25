#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Button.h"
#include "Score.h"
#include "Phase.h"

Button::Button(bool teamNumber, uint8_t buttonPin, Controller* controller)
{
  pinMode(buttonPin, INPUT);
  _teamNumber = teamNumber;
  _buttonPin = buttonPin;
  _controller = controller;
}

/**
 * Button.update()
 * 
 * If the button is pressed, update the controlling team in Score
 */
void Button::update() {
  // if the button was pressed last tick, and it is also pressed this tick, do nothing.
  if (digitalRead(_buttonPin) && _wasPressed) {
    return;
  }
  
  // if the button was not pressed last tick, but is pressed this tick, process press.
  else if (digitalRead(_buttonPin) && !_wasPressed) {
    processPress();
  }

  // if the button is not pressed this tick, but was pressed last tick, process release.
  else if (!digitalRead(_buttonPin) && _wasPressed) {
    
  }
}


/**
 * Do the action that the button should perform based on the current phase.
 */
void Button::processPress() {

  /**
   * Phase 0-- test phase. Button should advance to next phase when pressed
   */
  if (_controller->getCurrentPhase() == 0) {
    _controller->advancePhase();
    return;
  }

  /**
   * Phase 1-- Hello phase. Button should do nothing when pressed
   */
  else if (_controller->getCurrentPhase() == 1) {
    return;
  }


  /**
   * Phase 2-- Programming > Game mode.
   *   The phase where the type of game is chosen.
   *   Red button cycles through game modes.
   *   Green button selects game mode.
   */
  else if (_controller->getCurrentPhase() == 2) {

    // Red button cycles through game modes
    if (_teamNumber == 0) {
      // @TODO
    }
    else if (_teamNumber == 1) {
      _controller->advancePhase();
    }
  }


  /**
   * Phase 3-- Programming > Domination > duration.
   *   This is the phase when the user chooses the total cumulative time a team needs to control the point to win.
   *   Green button increments the time by 1 minute (up to a maximum of 595 hours)
   *   Red button decrements the time by 1 minute (down to a minimum of 1 second)
   */
  else if (_controller->getCurrentPhase() == 3) {
    
  }


  /**
   * Phase 4-- Domination > Run
   *   This is the phase where the Domination game is in progress.
   *   Red button sets team 0 as controlling
   *   Green button sets team 1 as controlling
   *   Button should set _wasPressed to TRUE, set controlling team in _score, and timestamp now as the button's last press time.
   */
  else if (_controller->getCurrentPhase() == 4) {
    _wasPressed = 1;
    _controller->triggerButtonPress(_teamNumber);
  }



  /**
   * Phase 5-- Domination > Pause
   *   Domination game mode is paused.
   *   Red button should not respond to presses or releases.
   *   Green button should not respond to presses or releases.
   *   Red and green buttons simultaneously pressed & held for 5 seconds should resume game (switch to Phase 4)
   */
  else if (_controller->getCurrentPhase() == 5) {
    // @todo
  }
    
  /**
   * Phase 6-- Domination > Win
   *   A team has won the game.
   *   Red button should not respond to presses or releases
   *   Green button should not respond to presses or releases
   *   Red and green buttons simultaneously pressed & held for 5 seconds should...
   *     * reset game scores
   *     * Go to phase 2
   */
   else if (_controller->getCurrentPhase() == 6) {
     // @todo
   }


   
}






/**
 * Do the release action that the button should perform based on the current phase.
 */
void Button::processRelease() {
  _wasPressed = 0;
}

