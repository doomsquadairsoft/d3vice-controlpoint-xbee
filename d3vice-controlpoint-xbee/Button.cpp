#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Button.h"
#include "Score.h"

Button::Button(bool teamNumber, uint8_t buttonPin, Score& score) :
  _score(score)
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
  if (digitalRead(_buttonPin)) {
    _score.setControllingTeam(_teamNumber);
  }
}

