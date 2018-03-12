#ifndef LED_h
#define LED_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Phase.h"

class LED
{
  public:
    LED(bool teamNumber, uint8_t ledPin, unsigned long duration);
    void update();
  private:
    bool _teamNumber;
    bool _isLEDOn;
    uint8_t _ledPin;
    unsigned long _unlitTime;
    unsigned long _duration;
};

#endif
