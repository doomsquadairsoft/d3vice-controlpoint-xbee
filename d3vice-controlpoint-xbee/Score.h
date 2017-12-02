#ifndef Score_h
#define Score_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif

class Score
{
  public:
    Score();
    void update();
    void setControllingTeam(bool teamNumber);
  private:
    long _team0Score;
    long _team1Score;
    bool _controllingTeam;
};


#endif
