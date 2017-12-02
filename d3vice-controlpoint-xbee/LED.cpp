#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "LED.h"
#include "Score.h"

LED::LED(bool teamNumber, uint8_t ledPin, unsigned long unitDuration, Score& score)
{
  _ledPin = ledPin;
  _teamNumber = teamNumber;
  _unlitDuration = unlitDuration
}

/**
 * LED.update()
 * 
 * If the button is pressed, temporarily turn off the corresponding LED
 */
void LED::update() {
  // If a button press for this LED's team has recently happened within the last _unlitDuration milliseconds,
  // turn off the led

  if (_score.getControllingTeam() == _teamNumber) {

    if (_score.getLastButtonPressTime() >= (millis() - _unlitDuration))
    digitalWrite(_ledPin, HIGH);
  }

  // if the 
  if () {
    
  }
}
