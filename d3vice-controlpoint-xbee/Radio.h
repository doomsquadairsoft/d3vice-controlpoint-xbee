#ifndef Radio_h
#define Radio_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include <XBee.h>
#include "Phase.h"



class Radio
{
  public:
    Radio(XBee& xbee, XBeeAddress64 gatewayAddress);
    void update();
    void greet();
    void _greet();
    bool isCurrentGame();
  private:
    XBee& _xbee;
    uint8_t _payload[3] = {67, 67, 67};
    uint8_t _payloadLength;
    unsigned long _lastGreetTime;
    XBeeAddress64 _gatewayAddress;
    ZBTxRequest _zbRequest;
    ZBTxStatusResponse _zbResponse;
};

#endif
