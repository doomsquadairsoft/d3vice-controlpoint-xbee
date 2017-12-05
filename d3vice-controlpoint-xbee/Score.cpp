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
}

void Score::update()
{
  // If a team is controlling the point, increment their score
  // @todo
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

