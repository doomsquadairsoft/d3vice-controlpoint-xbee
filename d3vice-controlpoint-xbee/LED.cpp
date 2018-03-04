#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "LED.h"
#include "Score.h"
#include "Phase.h"

LED::LED(bool teamNumber, uint8_t ledPin, unsigned long duration, Score* score, Phase* phase)
{
  pinMode(ledPin, OUTPUT);
  _ledPin = ledPin;
  _teamNumber = teamNumber;
  _duration = duration;
  _isLEDOn = 0;
  _unlitTime = 0;
  _score = score;
  _phase = phase;
}



/**
 * LED.update()
 * 
 * If the button is pressed, temporarily turn off the corresponding LED
 */
void LED::update() {
  
//  // if the button has never been pressed since d3vice start,
//  //   turn on the LED
//  if (_unlitTime == 0) {
//    if (!_isLEDOn) {
//      _isLEDOn = 1;
//      digitalWrite(_ledPin, HIGH);
//    }
//  }
//
//
//
//  // if the button was pressed within the last _duration milliseconds,
//  //   turn off the LED
//  if (millis() - _score->getLastButtonPressTime() < _duration) {
//    if (_phase->getWasSwitchedLastTick() == 1) {
//      if (_isLEDOn) {
//        _unlitTime = millis(); // save the timestamp at which the LED turned off
//        _isLEDOn = 0;
//        digitalWrite(_ledPin, LOW);
//      }
//    }
//  }
//
//  // if the button has not been pressed within the last _duration milliseconds,
//  //   turn on the LED
//  else if (millis() - _unlitTime > _duration) {
//    if (!_isLEDOn) {
//      _isLEDOn = 1;
//      digitalWrite(_ledPin, HIGH);
//    }
//  }
  

  
}
