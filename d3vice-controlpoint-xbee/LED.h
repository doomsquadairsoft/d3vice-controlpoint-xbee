#ifndef LED_h
#define LED_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Score.h"

class LED
{
  public:
    LED(bool teamNumber, uint8_t ledPin, unsigned long unitDuration, Score& score);
    void update();
  private:
    bool _teamNumber;
    bool _ledPin;
    unsigned long _unlitDuration;
    Score& _score;
};

#endif
