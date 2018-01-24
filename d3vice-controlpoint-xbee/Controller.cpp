#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Controller.h"
#include "Score.h"
#include "Phase.h"



Controller::Controller(Score* score, Phase* phase)
{
  _score = score;
  _phase = phase;
}

//Score& Controller::_score = _score;
//Phase& Controller::_phase = _phase;


void Controller::advancePhase() {
  _phase->advance();
}

void Controller::triggerButtonPress(bool buttonNumber) {
  _score->setControllingTeam(buttonNumber);
  _score->setLastButtonPressTime(millis()); // @todo is there a better class this state can be put in?
}

uint8_t Controller::getCurrentPhase() {
  return _phase->getCurrentPhase();
}

