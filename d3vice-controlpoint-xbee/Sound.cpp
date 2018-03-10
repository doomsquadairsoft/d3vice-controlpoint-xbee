#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Sound.h"
#include "Phase.h"


Sound::Sound(uint8_t buzzerPin) : _buzzerPin(buzzerPin) {}



void Sound::update()
{
  // if the buzzer is buzzing
  //   if the buzzer needs to stop buzzing
  //     stop the buzzer
  asyncBeep();

  // if this is the first tick of a new phase
  //   start beeping
  // @todo
  
}


void Sound::asyncBeep(uint32_t duration)
{
  // if the function was supplied a duration, set a timer for that duration and start beeping.
  _asyncBeepStartTime = millis();
  _asyncBeepDuration = duration;
  digitalWrite(_buzzerPin, HIGH);
  return;
}

void Sound::asyncBeep()
{
  // if the function was not supplied a duration, continue beeping if duration has not expired
  // otherwise, stop beeping
  if (millis() - _asyncBeepStartTime > _asyncBeepDuration) {
    digitalWrite(_buzzerPin, LOW);
  }
}





