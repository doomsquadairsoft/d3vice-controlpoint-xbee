#ifndef Duration_h
#define Duration_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif




class Duration
{
  public:
    Duration(uint8_t initialDuration);
    void set(uint8_t duration);
    uint8_t get();
    void increment();
    void decrement();
  private:
    uint8_t _duration;
};


#endif
