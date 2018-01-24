#ifndef LED_h
#define LED_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Controller.h"

class LED
{
  public:
    LED(bool teamNumber, uint8_t ledPin, unsigned long duration, Score& score, Phase& phase);
    void update();
  private:
    bool _teamNumber;
    bool _isLEDOn;
    uint8_t _ledPin;
    unsigned long _unlitTime;
    unsigned long _duration;
    static Score& _score;
    static Phase& _phase;
};

#endif
