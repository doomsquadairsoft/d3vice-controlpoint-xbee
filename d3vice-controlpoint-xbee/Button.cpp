#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Button.h"
#include "Score.h"
#include "Phase.h"

Button::Button(uint8_t teamNumber, uint8_t buttonPin, Phase& phase, Score& score)
{
  _teamNumber = teamNumber;
  _buttonPin = buttonPin;
  _score = score;
  _wasPressed = 0;
  _wasHeld = 0;
  _lastPressTime = 0;
  _lastReleaseTime = 0;
}

/**
 * Button.update()
 * 
 * If the button is pressed, update the controlling team in Score
 */
void Button::update() {

  
  // if the button was pressed last tick, and it is also pressed this tick, do nothing.
  if (digitalRead(_buttonPin) && _wasPressed) {

    // if the button is pressed and has been for 1 second, mark as held
    if (millis() - _lastPressTime > 1000 && !_wasHeld) {
      processHold();
    }
    
    return;
  }
  
  // if the button was not pressed last tick, but is pressed this tick, process press.
  else if (digitalRead(_buttonPin) && !_wasPressed) {
    processPress();
  }

  // if the button is not pressed this tick, but was pressed last tick, process release.
  else if (!digitalRead(_buttonPin) && _wasPressed) {
    processRelease();
  }


}


/**
 * Do the action that the button should perform based on the current phase.
 */
void Button::processPress() {



  // set the wasPressed boolean to TRUE which will be queried in later ticks to see if the button state was changed since last tick
  _wasPressed = 1;
  

  // set a start press time to later be used to determine if the button is being held (multi-button press&hold functionality)
  _lastPressTime = millis();
  
  
  /**
   * Phase 0-- test phase. Button should advance to next phase when pressed
   */
  if (_phase.getCurrentPhase() == 0) {
    if (_teamNumber == 0) {
      _phase.advance();
      digitalWrite(9, HIGH);
      delay(10);
      digitalWrite(9, LOW);
      delay(20);
    }
    else {
      digitalWrite(9, HIGH);
      delay(100);
      digitalWrite(9, LOW);
      delay(20);
    }
    return;
  }

  /**
   * Phase 1-- Hello phase. Button should do nothing when pressed
   *   phase advancement is handled by the XBee module.
   */
  else if (_phase.getCurrentPhase() == 1) {
    return;
  }







  /**
   * Phase 4-- Domination > Run
   *   This is the phase where the Domination game is in progress.
   *   Red button sets team 0 as controlling
   *   Green button sets team 1 as controlling
   *   Button should set _wasPressed to TRUE, set controlling team in _score, and timestamp now as the button's last press time.
   */
  else if (_phase.getCurrentPhase() == 4) {
    //_controller->triggerButtonPress(_teamNumber);
  }



  /**
   * Phase 5-- Domination > Pause
   *   Domination game mode is paused.
   *   Red button should not respond to presses or releases.
   *   Green button should not respond to presses or releases.
   *   Red and green buttons simultaneously pressed & held for 5 seconds should resume game (switch to Phase 4)
   */
  else if (_phase.getCurrentPhase() == 5) {
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
   else if (_phase.getCurrentPhase() == 6) {
     // @todo
   }


   
}






/**
 * Do the release action that the button should perform based on the current phase.
 */
void Button::processRelease() {

    
  _wasPressed = 0;
  _wasHeld = 0;
  _lastReleaseTime = millis();


  for(int i=0; i<1; i++) {
    digitalWrite(9, HIGH);
    delay(15);
    digitalWrite(9, LOW);
    delay(15);
  }
  delay(100);

  
  /**
   * Phase 2-- Programming > Game mode.
   *   The phase where the type of game is chosen.
   *   Red button cycles upwards through game modes.
   *   Green button cycles downward through game modes
   */
  if (_phase.getCurrentPhase() == 2) {
    if (_teamNumber == 0) {
      _score.selectNextGame();
    }
    else if (_teamNumber == 1) {
      _score.selectPreviousGame();
    }
  }


  /**
   * Phase 3-- Programming > Domination > duration.
   *   This is the phase when the user chooses the total cumulative time a team needs to control the point to win.
   *   Red button decrements the time by 1 minute (down to a minimum of 1 second)
   *   Green button increments the time by 1 minute (up to a maximum of 595 hours)
   */
  else if (_phase.getCurrentPhase() == 3) {
    if (_teamNumber == 0) {
      _score.incrementTimeToWin(60000);
    }
    else {
      _score.decrementTimeToWin(60000);
    }
  }
  
}




/**
 * processHold
 * 
 * the action to do when the button is held
 * 
 * This function is for single-button holds only.
 * multi-button holds are handled in ButtonManager.
 */
void Button::processHold() {
  _wasHeld = 1;


  /**
   * Phase 2-- Programming > Game mode.
   *   The phase where the type of game is chosen.
   *   Holding button 0 
   */
  if (_phase.getCurrentPhase() == 2) {
    //
  }
}


/**
 * getState
 * 
 * 0 released
 * 1 pressed
 * 2 held
 */
int Button::getState() {
  
  // if the latest press was more recent than the latest release, the button is physically pressed
  if (digitalRead(_buttonPin) == HIGH) {

    // if the button has been pressed for less than 1000 ms, consider it pressed
    if (millis() - _lastPressTime < 1000) {
      return 1;
    }

    // if the button has been pressed for greater than or equal to 1000 ms, consider it held
    else {
      return 2;
    }
  }

  // if the latest release was more recent than the latest press, the button is released.
  else {
    return 0;
  }
}

