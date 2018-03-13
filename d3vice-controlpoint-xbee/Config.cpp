#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Config.h"




Config::Config(uint8_t initialConfig = 10)
{
  _config = initialConfig;
}

void Config::set(uint8_t config)
{
  _config = config;  
}

void Config::increment()
{
  if (_config == 255) {
    _config = 0;
  }
  else {
    _config += 1;
  }
}

void Config::decrement()
{
  if (_config == 0) {
    _config = 255;
  }
  else {
    _config -= 1;
  }
}

uint8_t Config::get()
{
  return _config;
}

