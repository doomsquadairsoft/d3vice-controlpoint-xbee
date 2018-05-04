/**
 * d3vice-controlpoint-xbee
 *
 * DESCRIPTION
 *   Arduino Fio / XBee based D3VICE control point for Airsoft games.
 *   Makes king of the hill or Battlefield style sector control games easy.
 *
 *
 * PINOUTS
 *
 *   D4  Button 1
 *   D7  Button 2
 *   D9  Piezo Speaker
 *   D10 Button 1 LED
 *   D11 Button 2 LED
 *   D12 Neopixel
 *   D13 Onboard LED
 *
 *
 * LICENSE
 *   The Unlicense <https://unlicense.org>
 *
 *
 * AUTHOR
 *   Chris Grimmett <chris@grimtech.net>
 *
 *
 * CODE REPOSITORY
 *   https://github.com/doomsquadairsoft/d3vice-controlpoint-xbee
 *
 *
 */


/** 
 * Contributed libraries
 */
#include <Adafruit_NeoPixel.h>
#include <XBee.h>

/**
 * DooM Squad Libraries
 */
#include "Button.h"



// pin definitions
#define button0Pin 7
#define button1Pin 4
#define button0LEDPin 10
#define button1LEDPin 11
#define onboardLEDPin 13
#define neopixelPin 12
#define buzzerPin 9
#define sleepPin 6


/**
 * XBee configuration
 *
 * Here is the serial number of the destination XBee gateway
 */
uint32_t xbeeGatewaySH = 0x0013A200;
uint32_t xbeeGatewaySL = 0x40B774EC;

/**
 * xbee status led config (temporary @todo deleteme)
 */
int statusLed = button0LEDPin;
int errorLed = button1LEDPin;


/**
 * Phase
 */
int phase = 0;


/**
 * Timers
 */
unsigned long lastBroadcastHoldEvent = 0;
unsigned long lastXEvent = 0;


/**
 * Battlefield State
 */
uint8_t controllingTeam = 0;
uint8_t redProgress = 0;
uint8_t bluProgress = 0;


/**
 * instantiate class objects
 */
XBee xbee = XBee();
uint8_t payloadLength = 10;
uint8_t payload[] = { 48 };
unsigned long lastGreetTime = 0;
XBeeAddress64 gatewayAddress = XBeeAddress64(xbeeGatewaySH, xbeeGatewaySL);
ZBTxRequest zbRequest = ZBTxRequest(gatewayAddress, payload, payloadLength); // address, payload, size
ZBTxStatusResponse zbResponse = ZBTxStatusResponse();
ZBRxResponse rx = ZBRxResponse();
Button team0Button = Button(0, button0Pin);
Button team1Button = Button(1, button1Pin);


// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, neopixelPin, NEO_GRB + NEO_KHZ800);



/** 
 * configuration of the team's color (for neopixels)
 */
uint32_t redColor = strip.Color(255, 0, 0);
uint32_t bluColor = strip.Color(0, 0, 255);
uint32_t greyColor = strip.Color(50, 50, 50);










void setup() {


  pinMode(onboardLEDPin, OUTPUT);
  pinMode(button0LEDPin, OUTPUT);
  pinMode(button1LEDPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(neopixelPin, OUTPUT);
  pinMode(button0Pin, INPUT);
  pinMode(button1Pin, INPUT);
  pinMode(sleepPin, OUTPUT);
  digitalWrite(sleepPin, LOW);

  Serial.begin(57600);
  xbee.setSerial(Serial);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'




}











/**
 *
 * Main loop. One pass through the loop is called a "tick"
 *
 */
void loop()
{
  // poll pushbuttons for activity
  team0Button.update();
  team1Button.update();
  xbee.readPacket();

  
  if (phase == 0) {
    runPhase0();
  }

  else if (phase == 1) {
    runPhase1();
  }

  else if (phase == 2) {
    runPhase2();
  }

  else if (phase == 3) {
    runPhase3();
  }

  else if (phase == 4) {
    runPhase4();
  }

  else if (phase == 7) {
    runPhase7();
  }

  else if (phase == 19) {
    runPhase19();
  }

}



void flashLed(int pin, int times, int wait) {

  for (int i = 0; i < times; i++) {
    digitalWrite(pin, HIGH);
    delay(wait);
    digitalWrite(pin, LOW);

    if (i + 1 < times) {
      delay(wait);
    }
  }
}


void runPhase0() {
  
  // run a test sequence which the user can observe to verify that all the
  // lights and sounds are functioning properly.
  testSequence();
  phase += 1;
}

/**
 * PHASE 1
 * HELLO PHASE
 * 
 * Send HI until a gateway tells us what to do
 */
void runPhase1() { 

  /**
   * Send a HELLO
   */
  payload[0] = 'D';
  payload[1] = 'C';
  payload[2] = 'X';
  payload[3] = 'H';
  payload[4] = 'I';

  xbee.send(zbRequest);

  // flash TX indicator
  flashLed(statusLed, 1, 100);

  // after sending a tx request, we expect a status response
  // wait up to half second for the status response
  if (xbee.readPacket(500)) {
    // got a response!

    // should be a znet tx status              
    if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      xbee.getResponse().getZBTxStatusResponse(zbResponse);

      // get the delivery status, the fifth byte
      if (zbResponse.getDeliveryStatus() == SUCCESS) {
        // success.  time to celebrate
        flashLed(statusLed, 5, 50);
      }
      
      else {
        // the remote XBee did not receive our packet. is it powered on?
        flashLed(errorLed, 3, 500);
      }
    }
  } 
  
  else if (xbee.getResponse().isError()) {
    //nss.print("Error reading packet.  Error code: ");  
    //nss.println(xbee.getResponse().getErrorCode());
  } 
  
  else {
    // local XBee did not provide a timely TX Status Response -- should not happen
    flashLed(errorLed, 2, 50);
  }


  
  /**
   * Listen for orders from Controlpointer
   */
  xbee.readPacket();
  if (xbee.getResponse().isAvailable()) {
    // we got a response
    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
      xbee.getResponse().getZBRxResponse(rx);

      if (rx.getOption() == ZB_PACKET_ACKNOWLEDGED) {
        flashLed(statusLed, 10, 10);
      } 
      
      else {
        flashLed(errorLed, 2, 20);  
      }

      // interpret the orders
      if (
        rx.getData(0) == 'D' &&
        rx.getData(1) == 'C' &&
        rx.getData(2) == 'X' &&
        rx.getData(3) == 'G' &&
        rx.getData(4) == 'A' 
      )
      {
        // Our order is to start a game

        
        if (rx.getData(5) == '0') {
          // Our order is to start game 0 (Domination)
          phase = 4;
        }

        else if (rx.getData(5) == '1') {
          // Our order is to start game 1 (Controlpoint)
          phase = 19;
        }
      }
    }
  }
}


void runPhase2() {
  flashLed(statusLed, 2, 5);
}

void runPhase3() {
  flashLed(statusLed, 3, 50);
}

void runPhase4() {


  /**
   * If team 0 button is held, broadast
   */
  if (team0Button.getState() == 2) {
    broadcastHoldEvent(0);
  }

  else if (team1Button.getState() == 2) {
    broadcastHoldEvent(1);
  }

  /**
   * Listen for state updates
   */
  listenForState();

  /**
   * update LEDs according to state
   */
  displayState();
  
}


/**
 * button test phase
 * tests the various button states
 * 
 * 0 released
 * 1 pressed
 * 2 held
 * 3 locked
 *
 */
void runPhase7() {

  if (team0Button.getState() == 1 || 
      team1Button.getState() == 1) {
    flashLed(buzzerPin, 1, 100);
  }

  else if (team0Button.getState() == 2 ||
           team1Button.getState() == 2) {
    flashLed(buzzerPin, 2, 100);
  }

  else if (team0Button.getState() == 3 ||
           team1Button.getState() == 3) {
    flashLed(buzzerPin, 3, 100);
  }

  delay(300);

}

/**
 * PHASE 19
 * 
 * Battlefield (Running)
 */
void runPhase19() {
    flashLed(statusLed, 19, 50);
    

    // rate limiting
    if (millis() - lastXEvent > 1000) {
 

  }
}


void broadcastHoldEvent(int team) {
    // rate limiting
    if (millis() - lastBroadcastHoldEvent > 1000) {
    
      /**
       * Send a button hold event
       */
      payload[0] = 'D';
      payload[1] = 'C';
      payload[2] = 'X';
      payload[3] = 'B';
      payload[4] = 'H';
  
      if (team == 0) {
        payload[5] = '0';
      }
      else if (team == 1) {
        payload[5] = '1';
      }
      
    
      xbee.send(zbRequest);
    

      lastBroadcastHoldEvent = millis();
    }


    
    // flash TX indicator
    flashLed(statusLed, 1, 100);
  
    // after sending a tx request, we expect a status response
    // wait up to half second for the status response
    if (xbee.readPacket(1000)) {
      // got a response!
  
      // should be a znet tx status              
      if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
        xbee.getResponse().getZBTxStatusResponse(zbResponse);
  
        // get the delivery status, the fifth byte
        if (zbResponse.getDeliveryStatus() == SUCCESS) {
          // success.  time to celebrate
          flashLed(statusLed, 5, 100);
        } else {
          // the remote XBee did not receive our packet. is it powered on?
          flashLed(errorLed, 3, 500);
        }
      }
    } else if (xbee.getResponse().isError()) {
      //nss.print("Error reading packet.  Error code: ");  
      //nss.println(xbee.getResponse().getErrorCode());
    } else {
      // local XBee did not provide a timely TX Status Response -- should not happen
      flashLed(buzzerPin, 2, 100);
    }
}

//
//void doParse(&rx) {
//  for (uint8_t i=0; i<rx.getDataLength; i++) {
//    rx.getData(i);
//  }
//}

void listenForState() {  
  /**
   * Listen for state changes from controlpointer
   */
  xbee.readPacket();
  if (xbee.getResponse().isAvailable()) {
    // we got a response
    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
      xbee.getResponse().getZBRxResponse(rx);

      if (rx.getOption() == ZB_PACKET_ACKNOWLEDGED) {
        flashLed(statusLed, 10, 10);
      } 
      
      else {
        flashLed(errorLed, 2, 20);  
      }



      
      /**
       * interpret the state updates
       * 
       * https://github.com/doomsquadairsoft/d3vice-controlpoint-xbee/issues/2
       */
//      if (dcxParser.getDataType() == STATE_UPDATE) {
//        controllingTeam = dcxParser.getData(CONTROLLING_TEAM);
//        redProgress = dcxParser.getData(RED_PROGRESS);
//        bluProgress = dcxParser.getData(BLU_PROGRESS);
//      }


      
      if (
        rx.getData(0) == 'D' &&
        rx.getData(1) == 'C' &&
        rx.getData(2) == 'X' &&
        rx.getData(3) == 'S' &&
        rx.getData(4) == 'T' 
      )
      {
        // we're getting a state update (ST)
        //flashLed(buzzerPin, 6, 150);
        
        if (rx.getData(5) == 'C' &&
            rx.getData(6) == 'T'
        )
        {
          // Controlling Team
          //if (rx.getData(7) < 5) {
          if (rx.getData(7) == '0') {
            controllingTeam = 0;
          }
          else if (rx.getData(7) == '1') {
            controllingTeam = 1;
          }
          else if (rx.getData(7) == '2') {
            controllingTeam = 2;
          }

        }

        if (rx.getData(8) == 'R' &&
            rx.getData(9) == 'P'
        )
        {
          //if (rx.getData(10) < 101) {
            redProgress = rx.getData(10);
          //}
        }

        if (rx.getData(11) == 'B' &&
            rx.getData(12) == 'P'
        )
        {
          //if (rx.getData(13) < 101) {
            bluProgress = rx.getData(13);
          //}
        }
      }
    }
  }
}


/**
 * Display the state
 */
void displayState() {

  /**
   * If either redProgress or bluProgress has changed in the past 5 seconds,
   * show the progress bar of the team capturing the point
   */

//  // @TODO add timer which detets if bluProgress or redProgress has changed recently
//  if (!true) {
//    // show color coded progress bar on pixels 0-8
//    // show color coded progress bar on pixels 16-9
//
//    // if red controls, show blu progress bar
//    if (controllingTeam == 1) {
//      uint8_t mappedBluProgress = map(bluProgress, 0, 99, 0, 7);
//      
//      for(uint16_t i=0; i<mappedBluProgress; i++) {
//        strip.setPixelColor(i, bluColor);
//        strip.show();
//      }
//    }
//
//    // if blu controls, show red progress bar
//    else if (controllingTeam == 2) {
//      uint8_t mappedRedProgress = map(redProgress, 0, 99, 0, 7);
//      
//      for(uint16_t i=0; i<mappedRedProgress; i++) {
//        strip.setPixelColor(i, redColor);
//        strip.show();
//      }
//    }
//  }

  // bluProgress or redProgress has not changed within the last 5 seconds
  //else {
    
    /**
     * Show the color of the controlling team
     */
    if (controllingTeam == 0) {
      for(uint16_t i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, greyColor);
        strip.show();
      }
    }
  
    else if (controllingTeam == 1) {
      for(uint16_t i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, redColor);
        strip.show();
      }
    }
  
    else if (controllingTeam == 2) {
      for(uint16_t i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, bluColor);
        strip.show();
      }
    }

  //}



  
}






void testSequence() {


  // display all colors on neopixel strip
  digitalWrite(onboardLEDPin, HIGH);
  colorWipe(strip.Color(255, 0, 0), 20); // Red
  colorWipe(strip.Color(0, 255, 0), 20); // Green
  colorWipe(strip.Color(0, 0, 255), 20); // Blue
  delay(50);
  pixelsOff();
  digitalWrite(onboardLEDPin, LOW);

  delay(50);

  // blink the button LEDs
  digitalWrite(button0LEDPin, HIGH);
  digitalWrite(onboardLEDPin, HIGH);
  delay(50);
  digitalWrite(button0LEDPin, LOW);
  digitalWrite(onboardLEDPin, LOW);

  delay(50);

  digitalWrite(button1LEDPin, HIGH);
  digitalWrite(onboardLEDPin, HIGH);
  delay(50);
  digitalWrite(button1LEDPin, LOW);
  digitalWrite(onboardLEDPin, LOW);


  // blink the on-board LED
  digitalWrite(onboardLEDPin, HIGH);
  delay(50);
  digitalWrite(onboardLEDPin, LOW);

  delay(50);

  // beep the buzzer
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(onboardLEDPin, HIGH);
  delay(250);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(onboardLEDPin, LOW);
  delay(100);




}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void pixelsOff() {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
    strip.show();
  }
}

