#ifndef Sound_h
#define Sound_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

class Sound
{
  public:
    Sound(uint32_t duration);
    void update();
  private:
    uint32_t _duration;
};


#endif
