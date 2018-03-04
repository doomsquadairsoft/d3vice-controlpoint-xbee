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
    Button(uint8_t teamNumber, uint8_t buttonPin, Phase* phase, Score* score);
    void update();
    void processPress();
    void processRelease();
    void processHold();
    int getState();
  private:
    bool _wasHeld;
    bool _wasPressed;
    uint8_t _teamNumber;
    uint8_t _buttonPin;
    uint32_t _lastPressTime;
    uint32_t _lastReleaseTime;
    Score* _score;
    Phase* _phase;
};


#endif
