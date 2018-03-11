#ifndef GameMode_h
#define GameMode_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Score.h"




class GameMode
{
  public:
    GameMode(uint8_t initialGameMode);
    void set(uint8_t gameMode);
    uint8_t get();
    void increment();
    void decrement();
  private:
    uint8_t _gameMode;
};


#endif
