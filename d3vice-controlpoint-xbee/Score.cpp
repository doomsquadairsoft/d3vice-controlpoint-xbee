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

Score::update()
{
  :
}

Score::setControllingTeam(bool teamNumber)
{
  _controllingTeam = teamNumber;
}


