#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Sound.h"


Sound::Sound(uint32_t duration)
{
  _duration = duration;
}

void Sound::update()
{
  
}
