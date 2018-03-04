#ifndef ButtonManager_h
#define ButtonManager_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Button.h"


class ButtonManager
{
  public:
    ButtonManager(Button& team0Button, Button& team1Button, Phase* phase);
    ButtonManager(Button& team0Button, Button& team1Button, Phase& phase);
    void update();
  private:
    Button& _team0Button;
    Button& _team1Button;
    Phase* _phase;
};


#endif
