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
#include "ButtonManager.h"
#include "LED.h"
#include "LightStrip.h"
#include "Sound.h"
#include "Domination.h"
#include "Phase.h"
#include "Controller.h"
#include "Radio.h"



// pin definitions
#define button0Pin 4
#define button1Pin 7
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
Phase phase = Phase();
Domination game = Domination(score);
Controller controller = Controller(&score, &phase);

Radio radio = Radio(xbee, &phase, &controller);
Button team0Button = Button(5, button0Pin, &controller, &score);
Button team1Button = Button(0, button1Pin, &controller, &score);
ButtonManager buttonManager = ButtonManager(team0Button, team1Button, &phase);
LED button0LED = LED(0, button0LEDPin, 50, &score, &phase);
LED button1LED = LED(1, button1LEDPin, 50, &score, &phase);
LightStrip lightStrip = LightStrip(strip, &score, &phase);
Sound sound = Sound(buzzerPin, &phase);



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
  xbee.setSerial(Serial);
  
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
void loop() {


  
  // If either team has pressed their button,
  // register the appropriate event. (press/release)
  team0Button.update();
  team1Button.update();
  buttonManager.update();
  

  // Update the LEDs based on button presses
  // and the controlling team
  button0LED.update();
  button1LED.update();
  lightStrip.update();

  // Update the buzzer based on button presses
  sound.update();

  // Update the score based on the controlling team
  score.update();

  // Update the radio
  radio.update();

  // Update the phase
  // it is important to run this last, after all other modules.
  // Phase::update() handles resetting Phase::_isSwitchedLastTick to 0,
  // an important boolean which ensures proper buzzer operation
  phase.update();


  // @TODO XBee (wireless) stuff
  // SPEC: When D3VICE starts up, send HELLO out to network
  //       This is used by the gateway to sync the D3VICE's state with gamestate in case
  //       it rebooted during a game.



  // @TODO
  // SPEC: 



  if (team1Button.getState() == 0) {

  }

  else if (team1Button.getState() == 1) {

  }

  else if (team1Button.getState() == 2) {

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
