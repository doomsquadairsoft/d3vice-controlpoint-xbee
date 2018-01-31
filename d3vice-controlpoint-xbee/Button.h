#ifndef Button_h
#define Button_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Score.h"
#include "Phase.h"
#include "Controller.h"

class Button
{
  public:
    Button(bool teamNumber, uint8_t buttonPin, Controller* controller);
    void update();
    void processPress();
    void processRelease();
    void processHold();
    int getState();
  private:
    bool _wasHeld;
    bool _wasPressed;
    bool _teamNumber;
    uint8_t _buttonPin;
    uint32_t _lastPressTime;
    uint32_t _lastReleaseTime;
    Controller* _controller;
};


#endif
