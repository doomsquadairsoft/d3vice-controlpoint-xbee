#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "GameMode.h"




GameMode::GameMode(uint8_t initialGameMode = 0)
{
  _gameMode = initialGameMode;
}

void GameMode::set(uint8_t gameMode)
{
  _gameMode = gameMode;  
}

void GameMode::increment()
{
  if (_gameMode == 3) {
    _gameMode = 0;
  }
  else {
    _gameMode += 1;
  }
}

void GameMode::decrement()
{
  if (_gameMode == 0) {
    _gameMode = 3;
  }
  else {
    _gameMode -= 1;
  }
}

uint8_t GameMode::get()
{
  return _gameMode;
}

