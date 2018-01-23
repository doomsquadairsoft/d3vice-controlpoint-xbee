#ifndef Button_h
#define Button_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Score.h"
#include "Phase.h"

class Button
{
  public:
    Button(bool teamNumber, uint8_t buttonPin, Score& score, Phase& phase);
    void update();
    void processPress();
    void processRelease();
  private:
    bool _isPressed;
    bool _wasPressed;
    bool _teamNumber;
    uint8_t _buttonPin;
    Score& _score;
    Phase& _phase;
};


#endif
