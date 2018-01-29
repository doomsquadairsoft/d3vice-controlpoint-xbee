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
    uint8_t getState();
  private:
    bool _isPressed;
    bool _isReleased;
    bool _isHeld;
    bool _wasPressed;
    bool _teamNumber;
    uint8_t _buttonPin;
    uint32_t _startPressTime;
    Controller* _controller;
};


#endif
