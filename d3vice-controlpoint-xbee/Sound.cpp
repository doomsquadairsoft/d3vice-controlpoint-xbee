#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Sound.h"
#include "Score.h"


Sound::Sound(uint32_t duration, uint8_t buzzerPin, Score& score) :
  _score(score)
{
  _duration = duration;
  _buzzerPin = buzzerPin;
}

void Sound::update()
{
  // If a button press happened less than _duration ms ago,
  //   start buzzing
  if (millis() <= _score.getLastButtonPressTime() + _duration) {
    if (!digitalRead(_buzzerPin)) {
      digitalWrite(_buzzerPin, HIGH);
    }
  }

  
  // If the buzzer is buzzing
  //   and the time that a button was last pressed + _duration is greater than millis()
  //     stop buzzing 
  else {
    if (digitalRead(_buzzerPin)) {
      digitalWrite(_buzzerPin, LOW);
    }
  }
}
