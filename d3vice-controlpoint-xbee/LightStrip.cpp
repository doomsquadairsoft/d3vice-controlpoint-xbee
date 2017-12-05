#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "LightStrip.h"
#include <Adafruit_NeoPixel.h>
#include "Score.h"


LightStrip::LightStrip(Adafruit_NeoPixel& neoPixelStrip, Score& score) :
  _score(score), _neoPixelStrip(neoPixelStrip)
{
  _neoPixelStrip = neoPixelStrip;
  _isStarted = 0;
  _breathState = 0;
  _sinIn = 4.712;
  _isInhale = 0;
  _animationState = 0;
}

void LightStrip::update()
{
  // if the light strip has never been updated post-testSequence(),
  //   show the default animation
  if (_isStarted == 0) {
    _isStarted == 1;
    _animationState = 1;
  }

  _animate();
}

void LightStrip::_animate()
{
  // Animation state 0-- default state, nobody controls the point
  // shows yellow "breathing" color.
  if (_animationState == 1) {
    // sine wave loop
    // makes LEDs pulsate smoothly
    // greets https://www.sparkfun.com/tutorials/329
    float sinOutRed, sinOutGrn;

    // calculate the brightness of the LED "breath" using a sine wave
    // increase the period of the sine wave since last tick
    _sinIn = _sinIn + 0.01;

    // if the sine wave is at the end of the desired period (one phase),
    //   restart the phase
    if (_sinIn > 10.995) {
      _sinIn = 4.712;
    }

    // map the sine wave range (-1 to 1) to Red 0-204 and Green 0-150
    sinOutRed = sin(_sinIn) * 102 + 102;
    sinOutGrn = sin(_sinIn) * 76 + 76;

    for(uint16_t i=0; i<_neoPixelStrip.numPixels(); i++) {
      _neoPixelStrip.setPixelColor(i, _neoPixelStrip.Color(sinOutRed, sinOutGrn, 0));
      _neoPixelStrip.show();
    }
    
  }

  // Animation state 1-- team 0 just captured the point.
  // shows rapidly pulsating red color
  else if (_animationState == 2) {
    if (_isInhale) {
      if (_breathState < 255) {
        _breathState += 1;
      }
      else { 
        _isInhale = 0;
      }
    }
    else {
      if (_breathState > 0) {
        _breathState -= 1;
      }
      else {
        _isInhale = 1;
      }
    }
  }
  

  // Animation state 2-- team 0 controls the point.
  // shows red timer pattern
  

  // Animation state 3-- team 1 just captured the point.
  // shows rapidly pulsating green color

  // Animation state 4-- team 1 controls the point.
  // shows green timer pattern

  
}

