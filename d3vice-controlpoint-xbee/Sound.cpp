#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Sound.h"
#include "Score.h"
#include "Phase.h"


Sound::Sound(uint32_t duration, uint8_t buzzerPin, Score& score, Phase& phase) :
  _score(score), _phase(phase)
{
  _duration = duration;
  _buzzerPin = buzzerPin;
  _morseDitDuration = 100;
  _morseDahDuration = 350;
  _asyncBeepStartTime;
  _asyncMorseStartTime;
  _isAsyncBeepComplete = 0;
  _isAsyncMorseComplete = 0;
}

void Sound::update()
{

  // If the phase was just switched, reset the variables which prevent async beeps/morse sequences from occuring twice.
  if (_phase.getWasSwitchedLastTick() == 1) {
    _isAsyncBeepComplete = 0;
    _isAsyncMorseComplete = 0;
  }
  
  /**
  * Phase 0-- test phase. Buzzer should do a long beep
  */
  if (_phase.getCurrentPhase() == 0) {
    return;
  }
  
  
  /**
  * Phase 1-- Hello phase. Buzzer should do nothing
  */
  else if (_phase.getCurrentPhase() == 1) {
    return;
  }
  
  /**
  * Phase 2-- Programming > Game mode.
  *   The phase where the type of game is chosen.
  *   Buzzer should code "G" in morse to signal the user we are on (G)ame Mode selection
  */
  else if (_phase.getCurrentPhase() == 2) {
    // as long as we stay in this phase, start or continue transmitting the morse code character
    // we have to remember that this function is called hundreds (thousands?) of times a second
    // so it's possible that async morse will be cut off from finishing coding because a button was pressed.
    // therefore, we must ensure that on future calls to Sound in a different phase, we set the buzzerPin LOW.
    // Otherwise, an unfinished CW could beep indefinitely.
    asyncMorse('G');
  }
  
  
  
  // If a button press happened less than duration ms ago,
  //   start buzzing
  if (millis() - _score.getLastButtonPressTime() < _duration) {
    asyncBeep(_duration);
  }
}
  
    


void Sound::asyncMorse(char character) {
  _asyncMorseStartTime = millis();
}

void Sound::asyncBeep(uint32_t beepDuration) {
  _asyncBeepStartTime = millis();

  // if the beepDuration has not yet elapsed, continue beeping
  if (millis() - _asyncBeepStartTime < beepDuration) {
    digitalWrite(_buzzerPin, HIGH);
  }

  // stop buzzing once the beepDuration has elapsed
  else {
    if (digitalRead(_buzzerPin)) {
      digitalWrite(_buzzerPin, LOW);
    }
  }
}
  
   

