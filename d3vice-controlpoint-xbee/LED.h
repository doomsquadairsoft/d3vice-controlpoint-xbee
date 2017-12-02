#ifndef LED_h
#define LED_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

class LED
{
  public:
    LED(uint8_t ledPin);
    void update();
  private:
    bool _ledPin;
};

#endif
