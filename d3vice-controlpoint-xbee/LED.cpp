#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "LED.h"

LED::LED(uint8_t ledPin)
{
  _ledPin = ledPin;
}

/**
 * LED.update()
 * 
 * If the button is pressed, temporarily turn off the corresponding LED
 */
void LED::update() {
  
}
