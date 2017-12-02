#ifndef Button_h
#define Button_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Score.h"

class Button
{
  public:
    Button(bool teamNumber, uint8_t buttonPin, Score& score);
    void update();
  private:
    bool _teamNumber;
    uint8_t _buttonPin;
    Score& _score;
};

//extern Score;

#endif
