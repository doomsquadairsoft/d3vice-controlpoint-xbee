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
 * instantiate class objects
 */
XBee xbee = XBee();
uint8_t payloadLength = 5;
uint8_t payload[] = { 48, 48, 48, 48, 48 };
unsigned long lastGreetTime = 0;
XBeeAddress64 gatewayAddress = XBeeAddress64(xbeeGatewaySH, xbeeGatewaySL);
ZBTxRequest zbRequest = ZBTxRequest(gatewayAddress, payload, payloadLength); // address, payload, size
ZBTxStatusResponse zbResponse = ZBTxStatusResponse();
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
uint32_t team0color = strip.Color(255, 0, 0);
uint32_t team1color = strip.Color(0, 0, 255);










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
  // break down 10-bit reading into two bytes and place in payload
  payload[0] = 'D';
  payload[1] = 'C';
  payload[2] = 'X';

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
    flashLed(errorLed, 2, 50);
  }

  delay(1000);
  
}

void runPhase1() {
  flashLed(statusLed, 50, 50);
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

