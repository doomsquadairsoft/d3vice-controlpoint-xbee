#ifndef Config_h
#define Config_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif




class Config
{
  public:
    Config(uint8_t initialConfig);
    void set(uint8_t config);
    uint8_t get();
    void increment();
    void decrement();
  private:
    uint8_t _config;
};


#endif
