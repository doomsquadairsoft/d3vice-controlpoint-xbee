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


// Contributed libraries
#include <Adafruit_NeoPixel.h>
//#include <Printers.h>
#include <XBee.h>

// DooM Squad libraries
#include "Score.h"
#include "Button.h"
#include "LED.h"
#include "LightStrip.h"
#include "Sound.h"
#include "Domination.h"
#include "Phase.h"
#include "Radio.h"
#include "Tiesto.h"
//#include "Device.h"


// pin definitions
#define button0Pin 7
#define button1Pin 4
#define button0LEDPin 10
#define button1LEDPin 11
#define onboardLEDPin 13
#define neopixelPin 12
#define buzzerPin 9



// instantiate class objects
XBee xbee = XBee();

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, neopixelPin, NEO_GRB + NEO_KHZ800);

Score score = Score();
Phase phase = Phase(0);
Domination game = Domination(score);


// Device::Device(numberOfButtons, buzzerPin, strip, xbee)
//Device device = Device(1, buzzerPin, strip, xbee);

//Radio radio = Radio(xbee);
Button team0Button = Button(0, button0Pin);
Button team1Button = Button(1, button1Pin);
LED button0LED = LED(0, button0LEDPin, 50);
LED button1LED = LED(1, button1LEDPin, 50);
LightStrip lightStrip = LightStrip(strip);
Sound sound = Sound(buzzerPin);


// configuration of the team's color (for neopixels)
uint32_t team0color = strip.Color(255, 0, 0);
uint32_t team1color = strip.Color(0, 255, 0);


// counter for the time the teams have had control of the point
long team0controlTime = 0;
long team1controlTime = 0;


// variable for showing whether or not game is deriving initial state from XBee network
bool isNetworkGame;



void setup() {

 
  pinMode(onboardLEDPin, OUTPUT);
  pinMode(button0LEDPin, OUTPUT);
  pinMode(button1LEDPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(neopixelPin, OUTPUT);
  pinMode(button0Pin, INPUT);
  pinMode(button1Pin, INPUT);
  
  Serial.begin(57600);
  //xbee.setSerial(Serial);
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'


  // run a test sequence which the user can observe to verify that all the 
  // lights and sounds are functioning properly.
  testSequence();

  // @TODO 
  //   check the XBee network to determine if there is a running game already.
  isNetworkGame = 0;

  // if there is no game server on the XBee network, switch to autistic mode.
  // (standalone mode, no wireless connection active)
  if (isNetworkGame) {
    // @TODO
    //   Sync with the networked game state, then begin loop()
  }
  else {
    // @TODO
    //   enter autistic mode (standalone, no wireless connections active)
    
    // begin operation, starting with programming phase.
    return;
  }

  
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
  phase.update();

  for (uint8_t i=0; i<(phase.getCurrentPhase()+3); i++) {
    digitalWrite(9, HIGH);
    delay(20);
    digitalWrite(9, LOW);
    delay(150);
  }
  delay(1000);

  // do stuff based on the current phase
  if (phase.getCurrentPhase() == 0) {
    lightStrip.show(0);
  }

  else if (phase.getCurrentPhase() == 1) {
    lightStrip.show(1);
  }

  else if (phase.getCurrentPhase() == 2) {
    lightStrip.show(2);
  }

  else if (phase.getCurrentPhase() == 3) {
    lightStrip.show(3);
  }

  else if (phase.getCurrentPhase() == 4) {
    lightStrip.show(4);
  }
  
  else if (phase.getCurrentPhase() == 5) {
    lightStrip.show(5);
  }
  
  else if (phase.getCurrentPhase() == 6) {
    lightStrip.show(6);
  }


  

  
//  for (int i=0; i<17; i++) {
//    digitalWrite(9, HIGH);
//    delay(50);
//    digitalWrite(9, LOW);
//    delay(50);
//  }
//
//  delay(2000);
//
//  
//  for (uint8_t i=0; i<phase.getCurrentPhase(); i++) {
//    digitalWrite(9, HIGH);
//    delay(20);
//    digitalWrite(9, LOW);
//    delay(200);
//  }
//
//  delay(2000);
//
//  digitalWrite(9, HIGH);
//  delay(1000);
//  digitalWrite(9, LOW);
//  delay(1000);



  if (team1Button.getState() == 2) {
    for (uint8_t i=0; i<3; i++) {
      digitalWrite(9, HIGH);
      delay(20);
      digitalWrite(9, LOW);
      delay(200);
    }
    delay(1000);
  }

  

   /**
   * Phase 2-- Programming > Game mode.
   *   The phase where the type of game is chosen.
   *   Red button cycle forward through game modes
   *   Green button cycles backward throu game mode
   *   Both buttons held together advance to next phase
   */
  if (phase.getCurrentPhase() == 0) {
    // handle both buttons pressed simultaneously
    if (team0Button.getState() == 2 && team1Button.getState() == 2) {
      digitalWrite(9, HIGH);
      delay(2000);
      digitalWrite(9, LOW);
      phase.advance();
    }
  }


  /**
   * Phase 3-- Programming > Domination > duration.
   *   The phase when the user chooses the total cumulative time a team needs to control the point to win.
   *   Green button increments the time by 1 minute (up to a maximum of 595 hours)
   *   Red button decrements the time by 1 minute (down to a minimum of 1 second)
   *   Holding both buttons saves the selection and moves to the next phase (game running)
   */
   if (phase.getCurrentPhase() == 0) {
     if (team0Button.getState() == 2 && team1Button.getState() == 2) {
       phase.advance();
     }
   }

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
