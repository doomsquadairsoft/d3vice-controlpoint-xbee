#ifndef Button_h
#define Button_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Event.h"

class Button
{
  public:
    Button(bool teamNumber, uint8_t buttonPin, Event& event);
    void update();
  private:
    bool _isPressed;
    bool _wasPressed;
    bool _teamNumber;
    uint8_t _buttonPin;
    Event& _event;
};


#endif
