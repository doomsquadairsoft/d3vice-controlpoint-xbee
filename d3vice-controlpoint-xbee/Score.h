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
    void pause();
    void unPause();
    void setControllingTeam(bool teamNumber);
    void processCaptureEvent(bool teamNumber);
    bool getControllingTeam();
    long unsigned int getLastButtonPressTime();
    long unsigned int setLastButtonPressTime(long unsigned int lastButtonPressTime);
    long getTeamScore(bool teamNumber);
    void increment(bool teamNumber, unsigned long duration);
    bool getButtonState(bool buttonNumber);
    void incrementTimeToWin(unsigned long incrementValue);
    void decrementTimeToWin(unsigned long decrementValue);
    void selectNextGame();
    void selectPreviousGame();
    unsigned long getTimeToWin();
    uint16_t getMinutesToWin();
    uint8_t getSelectedGame();
  private:
    long _team0Score;
    long _team1Score;
    bool _controllingTeam;
    long unsigned int _lastButtonPressTime;
    bool _isGameInPlay;
    unsigned long _ttw;
    uint8_t _selectedGame;
};


#endif
