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
#include <Printers.h>
#include <XBee.h>

// DooM Squad libraries
#include "Score.h"
#include "Button.h"
#include "LED.h"
#include "LightStrip.h"
#include "Sound.h"



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
Button team0Button = Button(0, button0Pin, score);
Button team1Button = Button(1, button1Pin, score);
LED button0LED = LED(0, 10, 250, score);
LED button1LED = LED(1, 11, 250, score);
LightStrip lightStrip = LightStrip(neopixelPin, score);
Sound sound = Sound(1000, score);


// configuration of the team's color (for neopixels)
uint32_t team0color = strip.Color(255, 0, 0);
uint32_t team1color = strip.Color(0, 255, 0);


// counter for the time the teams have had control of the point
long team0controlTime = 0;
long team1controlTime = 0;



void setup() {
  pinMode(button0LEDPin, OUTPUT);
  pinMode(button1LEDPin, OUTPUT);
  pinMode(onboardLEDPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(neopixelPin, OUTPUT);
  Serial.begin(57600);
  xbee.setSerial(Serial);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  testSequence();
}


void loop() {

  
  // If either team has pressed their button,
  // change the controlling team in the score.
  team0Button.update();
  team1Button.update();

  // Update the LEDs based on button presses
  // and the controlling team
  button0LED.update();
  button1LED.update();
  lightStrip.update();

  // Update the buzzer based on button presses
  sound.update();

  // Update the score based on the controlling team
  score.update();


  // @TODO XBee (wireless) stuff
  // SPEC: When D3VICE starts up, send HELLO out to network
  //       This is used by the gateway to sync the D3VICE's state with gamestate in case
  //       it rebooted during a game.



  // @TODO
  // SPEC: 

  
}

void detectButtonState(bool team) {
  // detect if team n's button is pressed

  // if team n's button is pressed,
  //   return true
}

void processButtonPress(bool teamNumber) {
  // set controlling team to teamNumber
}

void processBeeps(long buttonLastPressTime) { 
  // if buttonLastPressTime happened in the last 1 second
  //   if we are not beeping already
  //     start beeping
  // otherwise
  //   if we are beeping
  //     stop beeping
}

void processLights(long buttonLastPressTime, bool teamNumber) {
  // if buttonLastPressTime 
}
  
  //   turn button teamNumber LED off
  //   change neopixel colour to team teamNumber's colour
  //   start beeping
  //   wait 1 second
  //   turn button teamNumber LED on
  //   stop beeping


void buttonPress(bool team) {

  
//  digitalWrite(onboardLEDPin, HIGH);
//  digitalWrite(button1LEDPin, HIGH);
//  digitalWrite(button2LEDPin, LOW);
//  if (longDelay) {
//    delay(2000);
//  } else {
//    delay(250);
//  }
//  digitalWrite(onboardLEDPin, LOW);
//  digitalWrite(button1LEDPin, LOW);
//  digitalWrite(button2LEDPin, HIGH);
//  if (longDelay) {
//    delay(2000);
//  } else {
//    delay(250);
//  }


}


void testSequence() {
  // display all colors on neopixel strip
  digitalWrite(onboardLEDPin, HIGH);
  colorWipe(strip.Color(255, 0, 0), 20); // Red
  colorWipe(strip.Color(0, 255, 0), 20); // Green
  colorWipe(strip.Color(0, 0, 255), 20); // Blue
  delay(250);
  pixelsOff();
  digitalWrite(onboardLEDPin, LOW);

  delay(250);

  // blink the button LEDs
  digitalWrite(button0LEDPin, HIGH);
  digitalWrite(onboardLEDPin, HIGH);
  delay(250);
  digitalWrite(button1LEDPin, LOW);
  digitalWrite(onboardLEDPin, LOW);

  delay(250);
  
  digitalWrite(button0LEDPin, HIGH);
  digitalWrite(onboardLEDPin, HIGH);
  delay(250);
  digitalWrite(button1LEDPin, LOW);
  digitalWrite(onboardLEDPin, LOW);


  // blink the on-board LED
  digitalWrite(onboardLEDPin, HIGH);
  delay(250);
  digitalWrite(onboardLEDPin, LOW);

  delay(250);

  // beep the buzzer
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(onboardLEDPin, HIGH);
  delay(1000);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(onboardLEDPin, LOW);
  delay(1000);
  

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

