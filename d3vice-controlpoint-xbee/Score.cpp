#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Score.h"


Score::Score()
{
  _team0Score = 0;
  _team1Score = 0;
  _isGameInPlay = 0;
  _ttw = 0;
}

void Score::update()
{
  // If a capture event has been recorded, 
  
  // If a team is controlling the point, increment their score
  // @todo
}

void Score::pause()
{
  if (_isGameInPlay == 1) _isGameInPlay = 0;
}

void Score::unPause()
{
  if (_isGameInPlay == 0) _isGameInPlay = 1;
}

void Score::setControllingTeam(bool teamNumber)
{
  _controllingTeam = teamNumber;
}

bool Score::getControllingTeam()
{
  return _controllingTeam;
}

unsigned long Score::getLastButtonPressTime()
{
  return _lastButtonPressTime;
}

unsigned long Score::setLastButtonPressTime(unsigned long lastButtonPressTime)
{
  _lastButtonPressTime = lastButtonPressTime;
  return _lastButtonPressTime;
}

long Score::getTeamScore(bool teamNumber)
{
  if (!teamNumber) {
    return _team0Score;
  }
  else {
    return _team1Score;
  }
}

void Score::processCaptureEvent(bool teamNumber)
{
   unPause();
   setControllingTeam(teamNumber);
   setLastButtonPressTime(millis());
}

void Score::increment(bool teamNumber, unsigned long duration)
{
    
}

void Score::incrementTimeToWin(unsigned long incrementValue)
{
  _ttw += incrementValue;
}

void Score::decrementTimeToWin(unsigned long decrementValue)
{
  if (_ttw - decrementValue >= 0) {
    _ttw -= decrementValue;
  }
  else {
    _ttw = 0;
  }
}

unsigned long Score::getTimeToWin()
{
  return 0;
  //return _ttw;
}

uint16_t Score::getMinutesToWin()
{
  uint16_t minutes = _ttw / 60000;
  return minutes;
}

