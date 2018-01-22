#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Button.h"
#include "Event.h"

Button::Button(bool teamNumber, uint8_t buttonPin, Event& event) :
  _event(event)
{
  pinMode(buttonPin, INPUT);
  _teamNumber = teamNumber;
  _buttonPin = buttonPin;
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
    // do the action that the button should perform.
    _event.registerPressEvent(_teamNumber);
  }

  // if the button is not pressed this tick, but was pressed last tick, process release.
  else if (!digitalRead(_buttonPin) && _wasPressed) {
    _wasPressed = 0;
    _event.registerReleaseEvent(_teamNumber);
  }
}


