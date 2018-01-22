#ifndef Event_h
#define Event_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Event.h"

class Event
{
  public:
    Event();
    void update();
    void registerPressEvent(bool teamNumber);
    void registerReleaseEvent(bool teamNumber);
    void registerLongHoldEvent(bool teamNumber);
    void registerLongReleaseEvent(bool teamNumber);
    void registerDoublePressEvent();
    void registerDoubleLongHoldEvent();
    void registerDoubleReleaseEvent();
    void registerDoubleLongReleaseEvent();
  private:
    bool _isPressEvent;
    bool _isReleaseEvent;
    bool _isLongHoldEvent;
    bool _isLongReleaseEvent;
    bool _isDoublePressEvent;
    bool _isDoubleLongHoldEvent;
    bool _isDoubleReleaseEvent;
    bool _isDoubleLongReleaseEvent;
};

#endif
