#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Button.h"
#include "Score.h"
#include "Phase.h"


Button::Button(uint8_t teamNumber, uint8_t buttonPin)
{
  _teamNumber = teamNumber;
  _buttonPin = buttonPin;
  _wasPressed = 0;
  _wasHeld = 0;
  _lastPressTime = 0;
  _lastReleaseTime = 0;
  _isLocked = 0;
}

/**
 * Button.update()
 * 
 * If the button is pressed, update the controlling team in Score
 */
void Button::update() {

  // reset toggle variables which only last for 1 tick
  if (_wasPressReleasedLastTick) { _wasPressReleasedLastTick = 0; }
  if (_wasHoldReleasedLastTick) { _wasHoldReleasedLastTick = 0; }

  
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

  // if the button is not pressed this tick, but was pressed last tick, process press release.
  else if (!digitalRead(_buttonPin) && _wasPressed) {
    processPressRelease();
  }

  // if the button is not pressed this tick, but was held last tick, process hold release.
  else if (!digitalRead(_buttonPin) && _wasHeld) {
    processHoldRelease();
  }

  


}


/**
 * Do the action that the button should perform based on the current phase->
 */
void Button::processPress() {
//  // debug. determine the phase that the button class thinks it is
//  for (int i=0; i<_phase->getCurrentPhase(); i++) {
//      digitalWrite(9, HIGH);
//      delay(10);
//      digitalWrite(9, LOW);
//      delay(50);
//  }




  // set the wasPressed boolean to TRUE which will be queried in later ticks to see if the button state was changed since last tick
  _wasPressed = 1;
  

  // set a start press time to later be used to determine if the button is being held (multi-button press&hold functionality)
  _lastPressTime = millis();

   
}






/**
 * Do the release action that the button should perform based on the current phase->
 */
void Button::processPressRelease() {

    
  _wasPressed = 0;
  _wasHeld = 0;
  _lastReleaseTime = millis();
  _isLocked = 0;
  _wasPressReleasedLastTick = 1;

//  if (_teamNumber == 0) {
//  
//    for(int i=0; i<1; i++) {
//      digitalWrite(9, HIGH);
//      delay(15);
//      digitalWrite(9, LOW);
//      delay(15);
//    }
//    delay(100);
//  }
//
//  else {
//        for(int i=0; i<1; i++) {
//      digitalWrite(9, HIGH);
//      delay(115);
//      digitalWrite(9, LOW);
//      delay(15);
//    }
//    delay(100);
//  }

  
}


void Button::processHoldRelease() {
  _wasPressed = 0;
  _wasHeld = 0;
  _lastReleaseTime = millis();
  _isLocked = 0;
  _wasHoldReleasedLastTick = 1;
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


}


/**
 * getState
 * 
 * 0 released
 * 1 pressed
 * 2 held
 * 3 locked
 */
int Button::getState() {

  if (_isLocked) {
    return 3;
  }
  
  // if the latest press was more recent than the latest release, the button is physically pressed
  else if (digitalRead(_buttonPin) == HIGH) {

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


/**
 * Prevent Button from returning anything except state 3.
 * Avoids problem where a press and hold triggers two phase changes rapid-fire.
 * Call this after the desired action was performed.
 * No other action can occur until button state 0 (button not pressed) occurs.
 * 
 */
void Button::lock()
{
  _isLocked = 1;
}





bool Button::wasPressReleasedLastTick()
{
  return _wasPressReleasedLastTick;
}

bool Button::wasHoldReleasedLastTick()
{
  if (_wasHoldReleasedLastTick) {
    return 1;
  }
  else {
    return 0;
  }
}

