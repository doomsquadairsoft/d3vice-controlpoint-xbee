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
    Button(uint8_t teamNumber, uint8_t buttonPin);
    void update();
    void processPress();
    void processPressRelease();
    void processHoldRelease();
    void processHold();
    void lock();
    bool wasHoldReleasedLastTick();
    bool wasPressReleasedLastTick();
    int getState();
  private:
    bool _wasHeld;
    bool _wasPressed;
    bool _isLocked;
    bool _wasPressReleasedLastTick;
    bool _wasHoldReleasedLastTick;
    uint8_t _teamNumber;
    uint8_t _buttonPin;
    uint32_t _lastPressTime;
    uint32_t _lastReleaseTime;
};


#endif
