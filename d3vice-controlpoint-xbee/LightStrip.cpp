#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "LightStrip.h"
#include <Adafruit_NeoPixel.h>
#include "Score.h"
#include "Phase.h"


LightStrip::LightStrip(Adafruit_NeoPixel& neoPixelStrip, Score& score, Phase* phase) :
  _neoPixelStrip(neoPixelStrip), _score(score)
{
  _neoPixelStrip = neoPixelStrip;
  _isStarted = 0;
  _breathState = 0;
  _sinIn = 4.712;
  _isInhale = 0;
  _phase4AnimationState = 0;
  _phase = phase;
}

void LightStrip::update()
{

//  // debug
//  for(uint16_t i=0; i<_neoPixelStrip.numPixels(); i++) {
//    _neoPixelStrip.setPixelColor(i, _neoPixelStrip.Color(0, 0, 0));
//  }
//  if (_phase->getWasSwitchedLastTick() == 1) {
//    _neoPixelStrip.setPixelColor(0, _neoPixelStrip.Color(25, 25, 25));
//  } else {
//    _neoPixelStrip.setPixelColor(1, _neoPixelStrip.Color(17, 0, 21));
//  }
//  _neoPixelStrip.show();
//  return;
  
  /**
   * Phase 0
   * 
   * Test phase
   * LightStrip should cycle through RGB colors to show all pixels are functioning properly
   */
  if (_phase->getCurrentPhase() == 0) {
    // @TODO
    for(uint16_t i=0; i<_neoPixelStrip.numPixels(); i++) {
      _neoPixelStrip.setPixelColor(i, _neoPixelStrip.Color(25, 0, 0));
      _neoPixelStrip.show();
    }
  }

  /**
   * Phase 1
   * 
   * Hello phase
   * LightStrip should do nothing
   */
  else if (_phase->getCurrentPhase() == 1) {
    for(uint16_t i=0; i<_neoPixelStrip.numPixels(); i++) {
      _neoPixelStrip.setPixelColor(i, _neoPixelStrip.Color(0, 25, 0));
      _neoPixelStrip.show();
    }
    return;
  }

  /**
   * Phase 2
   * 
   * Programming > Game mode
   * Light strip should cycle through colors/patterns which symbolize the game modes
   */
  else if (_phase->getCurrentPhase() == 2) {
    for(uint16_t i=0; i<_neoPixelStrip.numPixels(); i++) {
      _neoPixelStrip.setPixelColor(i, _neoPixelStrip.Color(0, 0, 25));
      _neoPixelStrip.show();
    }

//      for(uint16_t i=0; i<_neoPixelStrip.numPixels(); i++) {
//        _neoPixelStrip.setPixelColor(i, _neoPixelStrip.Color(0, 0, 0));
//      }
//      if (_phase->getWasSwitchedLastTick() == 1) {
//        _neoPixelStrip.setPixelColor(0, _neoPixelStrip.Color(25, 25, 25));
//      } else {
//        _neoPixelStrip.setPixelColor(1, _neoPixelStrip.Color(52, 1, 63));
//      }
//      _neoPixelStrip.show();
      
    return;
  }

  /**
   * Phase 3
   * 
   * Programming > Domination > duration
   * LightStrip should display the user selected duration in ~~binary~~ a progress bar
   */
  else if (_phase->getCurrentPhase() == 3) {


    // clear all neopixels
    for(uint16_t i=0; i<_neoPixelStrip.numPixels(); i++) {
      _neoPixelStrip.setPixelColor(i, _neoPixelStrip.Color(0, 0, 0));
    }

    // light one neopixel per each minute in Score::_ttw
    for (uint16_t i=0; i<((_score.getMinutesToWin())-1); i++) {
      _neoPixelStrip.setPixelColor(i, _neoPixelStrip.Color(72, 19, 0));
    }
    _neoPixelStrip.show();

    return;
  }

  /**
   * Phase 4
   * 
   * Domination Game Run
   * LightStrip should show the controlling team's color
   */
  else if (_phase->getCurrentPhase() == 4) {
    if (_score.getLastButtonPressTime()) {
      // if team 0 is in control
      //   if team 0 hasn't captured the point within the last 5 seconds,
      //     switch to animation 2
      //   else if team 0 captured the point within the last 5 seconds,
      //     switch to animation 1
      if (!_score.getControllingTeam()) {
        if (millis() - _score.getLastButtonPressTime() > 5000) {
          _phase4AnimationState = 2;
        }
        else {
          _phase4AnimationState = 1;
        }
      }
    
      // else if team 1 is in control
      //   if team 1 hasn't captured the point within the last 5 seconds,
      //     switch to animation 4
      //   else if team 1 captured the point within the last 5 seconds,
      //     switch to animation 3
      else {
        if (millis() - _score.getLastButtonPressTime() > 5000) {
          _phase4AnimationState = 4;
        }
        else {
          _phase4AnimationState = 3;
        }
      }
    }
  
    _animatePhase4();
  }


  /**
   * Phaes 5
   * 
   * Domination paused
   * LightStrip should display a cycle of blinking lights
   *   * green blinking progress bar
   *   * red blinking progress bar
   *   * Yellow blinking(?) maybe maybe not
   */
  else if (_phase->getCurrentPhase() == 5) {
    // @TODO
    for(uint16_t i=0; i<_neoPixelStrip.numPixels(); i++) {
      _neoPixelStrip.setPixelColor(i, _neoPixelStrip.Color(52, 4, 101));
      _neoPixelStrip.show();
    }
  }

  /**
   * Phase 6
   * 
   * Domination game won
   * LightStrip should display a steady blinking color of the winning team
   */
  else if (_phase->getCurrentPhase() == 6) {
    // @TODO
    for(uint16_t i=0; i<_neoPixelStrip.numPixels(); i++) {
      _neoPixelStrip.setPixelColor(i, _neoPixelStrip.Color(33, 89, 138));
      _neoPixelStrip.show();
    }
  }


  /**
   * Phase ???
   * 
   * fall-through unknown phase.
   * this code should never run. If it does, there be bugs!
   */
  else {
    for(uint16_t i=0; i<_neoPixelStrip.numPixels(); i++) {
      _neoPixelStrip.setPixelColor(i, _neoPixelStrip.Color(10, 10, 10));
      _neoPixelStrip.show();
    }
  }

  
}





uint8_t LightStrip::_pulsate(uint8_t breathState) {
  if (_isInhale) {
    if (breathState < 255) {
      breathState += 4;
    }
    else { 
      _isInhale = 0;
    }
  }
  else {
    if (breathState > 0) {
      breathState -= 4;
    }
    else {
      _isInhale = 1;
    }
  }
  return breathState;
}



void LightStrip::_animatePhase4()
{
  // Animation state 0-- default state, nobody controls the point
  // shows yellow "breathing" color.
  if (_phase4AnimationState == 0) {
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
    // @todo A nice feature would be to improve this fade so it retains the truest color throughout the fade.
    //       The problem is that 204-0 will go faster than 150-0, thus the green isn't fading as fast as it should
    //       to maintain the spectrum difference as when it is in full-brightness.
    sinOutRed = sin(_sinIn) * 102 + 102;
    sinOutGrn = sin(_sinIn) * 76 + 76;

    for(uint16_t i=0; i<_neoPixelStrip.numPixels(); i++) {
      _neoPixelStrip.setPixelColor(i, _neoPixelStrip.Color(sinOutRed, sinOutGrn, 0));
      _neoPixelStrip.show();
    }
    
  }

  // Animation state 1-- team 0 just captured the point.
  // shows rapidly pulsating red color
  else if (_phase4AnimationState == 1) {
    _breathState = _pulsate(_breathState);
    for(uint16_t i=0; i<_neoPixelStrip.numPixels(); i++) {
      _neoPixelStrip.setPixelColor(i, _neoPixelStrip.Color(_breathState, 0, 0));
      _neoPixelStrip.show();
    }
  }
  

  // Animation state 2-- team 0 controls the point.
  // shows red timer pattern
  else if (_phase4AnimationState == 2) {
    // @todo
    for(uint16_t i=0; i<_neoPixelStrip.numPixels(); i++) {
      _neoPixelStrip.setPixelColor(i, _neoPixelStrip.Color(50, 0, 0));
      _neoPixelStrip.show();
    }
  }
  

  // Animation state 3-- team 1 just captured the point.
  // shows rapidly pulsating green color
  else if (_phase4AnimationState == 3) {
    _breathState = _pulsate(_breathState);
    for(uint16_t i=0; i<_neoPixelStrip.numPixels(); i++) {
      _neoPixelStrip.setPixelColor(i, _neoPixelStrip.Color(0, _breathState, 0));
      _neoPixelStrip.show();
    }
  }

  
  // Animation state 4-- team 1 controls the point.
  // shows green timer pattern
  else if (_phase4AnimationState == 4) {
    // @todo
    for(uint16_t i=0; i<_neoPixelStrip.numPixels(); i++) {
      _neoPixelStrip.setPixelColor(i, _neoPixelStrip.Color(0, 50, 0));
      _neoPixelStrip.show();
    }
  }
  
}


