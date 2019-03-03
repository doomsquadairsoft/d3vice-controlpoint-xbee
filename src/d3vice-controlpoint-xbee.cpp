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
 *   A4  Battery voltage sense
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
#include <JC_Button.h> // https://github.com/JChristensen/JC_Button


/**
 * DooM Squad Libraries
 */
//#include "Button.h"



// pin definitions
#define button0Pin 7
#define button1Pin 4
#define button0LEDPin 10
#define button1LEDPin 11
#define onboardLEDPin 13
#define neopixelPin 12
#define buzzerPin 9
#define sleepPin 6
#define batteryPin A4


/**
 * Constants
 */
const bool RED = 0;
const bool BLU = 1;
const int GRY = 2;
const int BTN_RELEASED = 0;
const int BTN_PRESSED = 1;
const int BTN_HELD = 2;
const int BTN_LOCKED = 3;



/**
 * XBee configuration
 *
 * Here are the addresses the controlpoint may need
 */
uint32_t xbeeBroadcastSH = 0x00000000;
uint32_t xbeeBroadcastSL = 0x0000FFFF;
uint32_t xbeeGatewaySH = 0x0013A200;
uint32_t xbeeGatewaySL = 0x40B774EC;

/**
 * xbee status led config (temporary @todo deleteme)
 */
int statusLed = button0LEDPin;
int errorLed = button1LEDPin;


/**
 * LED state
 */
uint8_t breathState = 0;
float sinIn = 4.712;
bool isInhale = 0;
bool isLit = 0;
uint16_t scannerPosition = 0;


/**
 * Phase
 */
int phase = 0; // startup (initial) phase
int lastPhase = 1;


/**
 * Timers
 */
unsigned long lastBroadcastHoldEvent = 0;
unsigned long lastStandbyTime = 0;
unsigned long lastAnimationTime = 0;
unsigned long lastXEvent = 0;
unsigned long timeToCapture = 5000; // the time it takes to capture a point.
unsigned long lastPhase19Check = 250;
unsigned long lastPhase25Check = 250;
unsigned long lastCaptureTime = 0;



/**
 * Sector Control State
 */
uint8_t lastControllingTeam = GRY;
uint8_t lastCappingTeam = GRY;
uint8_t redProgress = 0;
uint8_t bluProgress = 0;


/**
 * Battery voltage read state
 */
int analogInput = 0;
float vout = 0.0;
float vin = 0.0;
float R1 = 100000.00; // resistance of R1 (100K) -see text!
float R2 = 10000.00; // resistance of R2 (10K) - see text!
int value = 0;



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
Button team0Button = Button(button0Pin, 25, true, false);
Button team1Button = Button(button1Pin, 25, true, false);


// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, neopixelPin, NEO_GRB + NEO_KHZ800);



/**
 * configuration of the team's color (for neopixels)
 */
uint32_t redColorBlinding = strip.Color(255, 0, 0);
uint32_t bluColorBlinding = strip.Color(0, 0, 255);
uint32_t gryColorBlinding = strip.Color(50, 50, 50);

uint32_t redColorDim = strip.Color(85, 0, 0);
uint32_t bluColorDim = strip.Color(0, 0, 85);
uint32_t gryColorDim = strip.Color(17, 17, 17);
uint32_t ylwColorDim = strip.Color(20, 20, 0);
uint32_t blkColorOff = strip.Color(0, 0, 0);



void setup() {
        pinMode(onboardLEDPin, OUTPUT);
        pinMode(button0LEDPin, OUTPUT);
        pinMode(button1LEDPin, OUTPUT);
        pinMode(buzzerPin, OUTPUT);
        pinMode(neopixelPin, OUTPUT);
        pinMode(button0Pin, INPUT);
        pinMode(button1Pin, INPUT);
        pinMode(batteryPin, INPUT);
        pinMode(sleepPin, OUTPUT);
        digitalWrite(sleepPin, LOW);

        team0Button.begin();
        team1Button.begin();

        Serial.begin(57600);
        xbee.setSerial(Serial);

        strip.begin();
        strip.show(); // Initialize all pixels to 'off'

        /**
         * Init ADC
         */
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
        ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
        ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
        ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
        ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif
}






void setProgressBar(bool teamNumber, uint16_t percentage) {
        // determine how many pixels to turn on
        uint16_t split = (100 / strip.numPixels());
        uint16_t numberOfNeopixelsToLight = (percentage / split);

        // clear all neopixels
        for(uint16_t i=0; i<strip.numPixels(); i++) {
                strip.setPixelColor(i, strip.Color(0, 0, 0));
        }

        // light up the necessary neopixels
        if (!teamNumber) {
                // red goes right to left
                for(uint16_t i=0; i<numberOfNeopixelsToLight-1; i++) {
                        strip.setPixelColor(i, strip.Color(20, 0, 0));
                }
        } else {
                // blu goes left to right
                // teh bug is here vvvv
                uint16_t endPixel = strip.numPixels()-numberOfNeopixelsToLight;
                for(uint16_t i=strip.numPixels(); i>endPixel; i--) {
                        strip.setPixelColor(i, strip.Color(0, 0, 20));
                }
        }
        strip.show();
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



/**
 * readInternalVcc
 * Read the CHIP voltage. (not the battery voltage)
 * Read the 1.1V reference against AVcc
 * greetz https://provideyourown.com/2012/secret-arduino-voltmeter-measure-battery-voltage/ (voltage calculation code)
 * gratz http://maxembedded.com/2011/06/the-adc-of-the-avr/ (ADCSRA register explanation)
 */
long readInternalVcc() {
        #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
        ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
        #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
        ADMUX = _BV(MUX5) | _BV(MUX0);
        #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
        ADMUX = _BV(MUX3) | _BV(MUX2);
        #else
        ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
        #endif

        delay(2);
        ADCSRA |= _BV(ADSC); // Start conversion
        while (bit_is_set(ADCSRA,ADSC)); // loop, doing nothing until the measuring is complete
        uint8_t low = ADCL; // result of measurement is stored in ADCL (low) and ADCH (high)
        uint8_t high = ADCH;
        long result = (high<<8) | low;
        // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
        // this is subject to PER D3VICE calibration, as internal 1.1V reference on ATmega vary from 1.0 to 1.2 volts.
        // to calibrate, adjust 1125300L constant using the following formula--
        // scale_constant = internal1.1Ref * 1023 * 1000
        // To get internal1.1Ref, gather Vcc1 and Vcc2.
        // To get Vcc1, use a voltmeter to measure Arduino's Vcc.
        // To get Vcc2, run this function, readInternalVcc().
        // Then, internal1.1Ref can be calculated using the following formula--
        // internal1.1Ref = 1.1 * Vcc1 (per voltmeter) / Vcc2 (per readVcc() function)
        result = 1125300L / result;
        return result;
}

/**
 * Read voltage on Pin A3
 * Compare against 1.1V internal reference for accuracy
 */
float readExternalVcc() {
        value = analogRead(batteryPin);

        // these next lines are good for converting to a float.
        // since we are sending over xbee, we prefer an int.
        // vout = (value * 3.3) / 1024.0;
        // vin = vout / (R2/(R1+R2));
        // return vin;
        return value;
}


void decrementBluProgress() {
        // points are accumulated at a rate of
        // 255 points per n milliseconds
        // where n is timeToCapture.
        //
        // Because phase19 runs 4 times a second,
        // The algo which determines the amount to decrement per second is
        // 255 / 5000 = 0.051 points per ms
        // 0.051 * 1000 = 51 points per second
        // 51 / 4 = 12.75 points per 1/4 second

        // 255/n*250 = amount to decrement per 1/4 second (where n is timeToCapture)


        float delta = 255.0 / timeToCapture * 250;
        long result = bluProgress - delta;
        if (result <= 0) {
                bluProgress = 0;
        }

        else {
                bluProgress -= delta;
        }

}




void decrementRedProgress() {
        float delta = 255.0 / timeToCapture * 250;
        long result = redProgress - delta;
        if (result <= 0) {
                redProgress = 0;
        }

        else {
                redProgress -= delta;
        }
}




void incrementBluProgress() {
        uint8_t delta = 255.0 / timeToCapture * 250;
        long result = bluProgress + delta;

        if (result >= 255) {
                bluProgress = 255;
                lastCaptureTime = millis();
                lastControllingTeam = BLU;
        }

        else {
                bluProgress += delta;
        }
}





void incrementRedProgress() {


        uint8_t delta = 255.0 / timeToCapture * 250;
        long result = redProgress + delta;

        if (result >= 255) {
                redProgress = 255;
                lastCaptureTime = millis();
                lastControllingTeam = RED;
        }

        else {
                redProgress += delta;
        }
}


/**
 * @TODO
 *
 * Log a button/capture event to PROGMEM.
 * Later on when XBee connection is established,
 * Sync events with controlpointer for later scoring.
 */
void logEvent() {

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
                        } else {
                                // the remote XBee did not receive our packet. is it powered on?
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
                        }

                        else {
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

                                if (rx.getData(5) == 'C' &&
                                    rx.getData(6) == 'T'
                                    )
                                {
                                        // Controlling Team
                                        //if (rx.getData(7) < 5) {
                                        if (rx.getData(7) == '0') {
                                                //controllingTeam = 0;
                                        }
                                        else if (rx.getData(7) == '1') {
                                                //controllingTeam = 1;
                                        }
                                        else if (rx.getData(7) == '2') {
                                                //controllingTeam = 2;
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
 * Pulsate
 *
 * Compute the brightness of the LED in a pulsating fashion.
 */
uint8_t pulsate(uint8_t breathState) {

        if (isInhale) {
                if (breathState < 63) {
                        breathState += 4;
                }
                else {
                        isInhale = 0;
                }
        }
        else {
                if (breathState > 0) {
                        breathState -= 4;
                }
                else {
                        isInhale = 1;
                }
        }
        return breathState;

}



/**
 * Display the state on the NeoPixels
 */
void displayState() {

        /**
         * If either redProgress or bluProgress has reached 255 in the past 5 seconds,
         * pulsate the respective color indicating team capture
         */
        if (lastCaptureTime != 0 && millis() - lastCaptureTime < 5000) {
                breathState = pulsate(breathState);
                for(uint16_t i=0; i<strip.numPixels(); i++) {

                        if (redProgress == 255) {
                                strip.setPixelColor(i, strip.Color(breathState, 0, 0));
                        }
                        else if (bluProgress == 255) {
                                strip.setPixelColor(i, strip.Color(0, 0, breathState));
                        }
                        strip.show();
                }
        }


        /**
         * No full capture has occured in the last 5 seconds.
         * If either team has full control (xProgress == 255), Show the color of the controlling team
         * Display grey if no team has any progress points
         */
        else if (
                (redProgress == 255 && bluProgress == 255) ||
                (redProgress == 0 && bluProgress == 0)
                ) {


                if (redProgress == 0 && bluProgress == 0) {
                        for(uint16_t i=0; i<strip.numPixels(); i++) {
                                strip.setPixelColor(i, gryColorDim);
                                strip.show();
                        }
                }

                else if (redProgress == 255) {
                        for(uint16_t i=0; i<strip.numPixels(); i++) {
                                strip.setPixelColor(i, redColorDim);
                                strip.show();
                        }
                }

                else if (bluProgress == 255) {
                        for(uint16_t i=0; i<strip.numPixels(); i++) {
                                strip.setPixelColor(i, bluColorDim);
                                strip.show();
                        }
                }
        }


        /**
         * No full capture has occured in the last 5 seconds.
         * If neither team has full control, show progress bars.
         */
        else {

                // @TODO Revise

                // show color coded progress bar on pixels 0-8
                // show color coded progress bar on pixels 16-9


                uint8_t mappedBluProgress = map(bluProgress, 0, 255, 0, strip.numPixels());
                uint8_t mappedRedProgress = map(redProgress, 0, 255, 0, strip.numPixels());



                // clear all pixels
                for(uint16_t i=0; i<strip.numPixels(); i++) {
                        strip.setPixelColor(i, gryColorDim);
                }


//    /**
//     * lastControllingTeam was RED which means this will be a red bar
//     */
//    if (lastControllingTeam == RED) {
//      for(uint16_t i=0; i<mappedRedProgress; i++) {
//        strip.setPixelColor(i, redColor);
//      }
//    }
//
//    else if (lastControllingTeam == BLU) {
//      for(uint16_t i=0; i<mappedBluProgress; i++) {
//        strip.setPixelColor(i, bluColor);
//      }
//    }


                //else {




                if (redProgress == 0) {
                        for (uint16_t i=0; i<mappedBluProgress; i++) {
                                strip.setPixelColor(i, bluColorDim);
                        }
                }

                else if (bluProgress == 0) {
                        for (uint16_t i=0; i<mappedRedProgress; i++) {
                                strip.setPixelColor(i, redColorDim);
                        }
                }
                //}

                strip.show();



        }
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

void radioSendPress(boolean teamNumber) {
        // configure the payload
        payload[0] = 'D';
        payload[1] = 'C';
        payload[2] = 'X';
        payload[3] = 'B';
        payload[4] = 'P';
        payload[5] = teamNumber;

        // send a warm greeting of radiation
        xbee.send(zbRequest);

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
                        } else {
                                // the remote XBee did not receive our packet. is it powered on?
                        }
                } else {
                }
        } else if (xbee.getResponse().isError()) {
                //nss.print("Error reading packet.  Error code: ");
                //nss.println(xbee.getResponse().getErrorCode());
        } else {
                // local XBee did not provide a timely TX Status Response -- should not happen
        }
}

void radioSendRelease(boolean teamNumber) {
        // configure the payload
        payload[0] = 'D';
        payload[1] = 'C';
        payload[2] = 'X';
        payload[3] = 'B';
        payload[4] = 'R';
        payload[5] = teamNumber;

        // send a warm greeting of radiation
        xbee.send(zbRequest);

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
                        } else {
                                // the remote XBee did not receive our packet. is it powered on?
                        }
                } else {
                }
        } else if (xbee.getResponse().isError()) {
                //nss.print("Error reading packet.  Error code: ");
                //nss.println(xbee.getResponse().getErrorCode());
        } else {
                // local XBee did not provide a timely TX Status Response -- should not happen
        }
}

void radioSendBatt(int vcc) {

        // configure the payload
        payload[0] = 'D';
        payload[1] = 'C';
        payload[2] = 'X';
        payload[3] = 'D';
        payload[4] = 'C';
        payload[5] = lowByte(vcc);
        payload[6] = highByte(vcc);

        // send a warm greeting of radiation
        xbee.send(zbRequest);


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
                        } else {
                                // the remote XBee did not receive our packet. is it powered on?
                        }
                } else {
                }
        } else if (xbee.getResponse().isError()) {
                //nss.print("Error reading packet.  Error code: ");
                //nss.println(xbee.getResponse().getErrorCode());
        } else {
                // local XBee did not provide a timely TX Status Response -- should not happen
        }
}



void radioSendGreet() {

        // configure the payload
        payload[0] = 'D';
        payload[1] = 'C';
        payload[2] = 'X';
        payload[3] = 'H';
        payload[4] = 'I';



        // send a warm greeting of radiation
        xbee.send(zbRequest);



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
                        } else {
                                // the remote XBee did not receive our packet. is it powered on?
                        }
                } else {
                }
        } else if (xbee.getResponse().isError()) {
                //nss.print("Error reading packet.  Error code: ");
                //nss.println(xbee.getResponse().getErrorCode());
        } else {
                // local XBee did not provide a timely TX Status Response -- should not happen
        }

}


void radioGreet() {
        // greet immediately if this is the first time a greet has occured.
        // this will only happen once per run.
        if (lastGreetTime == 0) {
                radioSendGreet();
                lastGreetTime = millis();
        }

        // greet once every 5 seconds
        else if (millis() - lastGreetTime >= 10000) {
                radioSendGreet();

                // log this greet occurance
                lastGreetTime = millis();
        }
}


void strobeLed() {
        if (millis() - lastAnimationTime >= 5000) {
                for(uint16_t i=0; i<strip.numPixels(); i++) {
                        strip.setPixelColor(i, strip.Color(84, 62, 0));
                }
                strip.show();
                lastAnimationTime = millis();
        }

        if (millis() - lastAnimationTime >= 50) {
                for(uint16_t i=0; i<strip.numPixels(); i++) {
                        strip.setPixelColor(i, strip.Color(0, 0, 0));
                }
                strip.show();
        }
}

void breatheLed() {
        // sine wave loop
        // makes LEDs pulsate smoothly
        // greets https://www.sparkfun.com/tutorials/329
        float sinOutRed, sinOutGrn;

        // calculate the brightness of the LED "breath" using a sine wave
        // increase the period of the sine wave since last tick
        sinIn = sinIn + 0.01;

        // if the sine wave is at the end of the desired period (one phase),
        //   restart the phase
        if (sinIn > 10.995) {
                sinIn = 4.712;
        }

        // map the sine wave range (-1 to 1) to Red 0-204 and Green 0-150
        // @todo A nice feature would be to improve this fade so it retains the truest color throughout the fade.
        //       The problem is that 204-0 will go faster than 150-0, thus the green isn't fading as fast as it should
        //       to maintain the spectrum difference as when it is in full-brightness.
        sinOutRed = sin(sinIn) * 102 + 102;
        sinOutGrn = sin(sinIn) * 76 + 76;

        for(uint16_t i=0; i<strip.numPixels(); i++) {
                strip.setPixelColor(i, strip.Color(sinOutRed, sinOutGrn, 0));
                strip.show();
        }

}



void animationScanner() {
        // run animation tick no more than every 50 ms
        if (millis() - lastAnimationTime >= 50) {
                // all pixels off
                for(uint16_t i=0; i<strip.numPixels(); i++) {
                        strip.setPixelColor(i, blkColorOff);
                }

                // on pixel on
                strip.setPixelColor(scannerPosition, ylwColorDim);
                strip.show();

                // update next pixel positoin
                if (scannerPosition == strip.numPixels()) {
                        scannerPosition = 0;
                }
                else {
                        scannerPosition += 1;
                }

                // reset animation timer
                lastAnimationTime = millis();
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
        delay(150);
        digitalWrite(button0LEDPin, LOW);
        digitalWrite(onboardLEDPin, LOW);

        delay(50);

        digitalWrite(button1LEDPin, HIGH);
        digitalWrite(onboardLEDPin, HIGH);
        delay(150);
        digitalWrite(button1LEDPin, LOW);
        digitalWrite(onboardLEDPin, LOW);


        // blink the on-board LED
        digitalWrite(onboardLEDPin, HIGH);
        delay(50);
        digitalWrite(onboardLEDPin, LOW);

        delay(50);


        setProgressBar(0, 0);
        delay(250);
        setProgressBar(1, 25);
        delay(250);
        setProgressBar(0, 50);
        delay(250);
        setProgressBar(1, 100);
        delay(250);
        setProgressBar(0, 0);
        delay(250);
        setProgressBar(1, 25);
        delay(250);
        setProgressBar(0, 50);
        delay(250);
        setProgressBar(1, 100);
        delay(250);
        setProgressBar(0, 75);
        delay(250);
        setProgressBar(1, 75);
        delay(250);
        setProgressBar(0, 0); // turn all neopixels off


        // beep the buzzer
        digitalWrite(buzzerPin, HIGH);
        digitalWrite(onboardLEDPin, HIGH);
        delay(250);
        digitalWrite(buzzerPin, LOW);
        digitalWrite(onboardLEDPin, LOW);
        delay(100);
}





void runPhase0() {

        // run a test sequence which the user can observe to verify that all the
        // lights and sounds are functioning properly.
        testSequence();
        //phase += 1;

        phase = 1; // We go straight to phase 1 where DooM HQ (or gateway) will tell this d3vice what to do
}

/**
 * PHASE 1
 * HELLO PHASE
 *
 * Send HI until a gateway tells us what to do
 */
void runPhase1() {

        animationScanner();

        /**
         * Send a HELLO
         */
        radioGreet();


        /**
         * Listen for orders from Controlpointer/DooM HQ
         */
        xbee.readPacket();
        if (xbee.getResponse().isAvailable()) {
                // we got a response
                if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
                        xbee.getResponse().getZBRxResponse(rx);

                        if (rx.getOption() == ZB_PACKET_ACKNOWLEDGED) {
                        }

                        else {
                        }

                        // interpret the orders.
                        if (
                                rx.getData(0) == 'D' &&
                                rx.getData(1) == 'C' &&
                                rx.getData(2) == 'X' &&
                                rx.getData(3) == 'G' &&
                                rx.getData(4) == 'A'
                                )
                        {
                                // DCXGA means our order is to start a game


                                if (rx.getData(5) == '0') {
                                        // Our order is to start game 0 (Domination)
                                        phase = 4;
                                }

                                else if (rx.getData(5) == '1') {
                                        // Our order is to start game 1 (Controlpoint)
                                        phase = 19;
                                }
                        }

                        // if we get a battery request (DC), respond with voltage
                        else if (
                                rx.getData(0) == 'D' &&
                                rx.getData(1) == 'C' &&
                                rx.getData(2) == 'X' &&
                                rx.getData(3) == 'D' &&
                                rx.getData(4) == 'C'
                                )
                        {
                                // DCXDC means go to BATT check phase (it returns afterwards)
                                lastPhase = 1;
                                phase = 8;
                        }

                        else if (
                                rx.getData(0) == 'D' &&
                                rx.getData(1) == 'C' &&
                                rx.getData(2) == 'X' &&
                                rx.getData(3) == 'S' &&
                                rx.getData(4) == 'B' &&
                                rx.getData(5) == 'Y'
                                )
                        {
                                // DCXSBY means our order is to standby
                                phase = 2;
                        }

                        else if (
                                rx.getData(0) == 'D' &&
                                rx.getData(1) == 'C' &&
                                rx.getData(2) == 'X' &&
                                rx.getData(3) == 'P' &&
                                rx.getData(4) == '2' &&
                                rx.getData(5) == '5'
                        )
                        {
                                // DCXP25 Means go to phase 25
                                phase = 25;
                        }
                }
        }
}





/**
 * Phase 2
 *
 * Standby phase
 *
 * Admin marked this device as PENDING or INACTIVE. Just sit and wait for orders.
 */
void runPhase2() {
        strobeLed();

        if (lastStandbyTime == 0) {
                lastStandbyTime = millis();

        }

        // Every minute, see if the game has orders for me
        if (millis() - lastStandbyTime >= 60000) {
                phase = 1; // send HELLO and see what orders the server gives me

                lastStandbyTime = 0;
        }
}


/**
 *
 */
void runPhase3() {
}

void runPhase4() {

        // deprecated
        // /**
        //  * If team 0 button is held, broadast
        //  */
        // if (team0Button.wasPressed()) {
        //         //broadcastHoldEvent(0);
        // }
        //
        // else if (team1Button.getState() == 2) {
        //         broadcastHoldEvent(1);
        // }

        // /**
        //  * Listen for state updates
        //  */
        // listenForState();
        //
        // /**
        //  * update LEDs according to state
        //  */
        // displayState();

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

        if (team0Button.wasPressed() ||
            team1Button.wasPressed()) {
        }

        else if (team0Button.wasPressed() ||
                 team1Button.wasPressed()) {
        }

        // else if (team0Button.getState() == 3 ||
        //          team1Button.getState() == 3) {
        // }

        delay(300);

}

/**
 * PHASE 8
 *
 * Battery voltage test phase
 */
void runPhase8() {
        int vcc = readExternalVcc();
        radioSendBatt(vcc);
        phase = lastPhase; // go back to the previous phase
}

/**
 * Phase 9
 * RSSI test phase
 * Returns to previous phase when done
 *
 */
void runPhase9() {

        if (xbee.getResponse().isAvailable()) {
                if (xbee.getResponse().getApiId() == RX_16_IO_RESPONSE) {

                }
        }
        if (team0Button.wasPressed() ||
            team1Button.wasPressed()) {
        }

        else if (team0Button.wasReleased() ||
                 team1Button.wasReleased()) {
        }

        delay(300);

        phase = lastPhase;
}



/**
 * PHASE 19
 *
 * Sector Control (Running)
 */
void runPhase19() {

        /**
         * Run every 1/4 second.
         *
         * If team RED button is held,
         *   increment redProgress if it isn't maxed out yet
         *
         */
        if (millis() - lastPhase19Check > 250) {

                // if red button is held, do stuff.
                if (team0Button.wasPressed()) {

                        lastCappingTeam = RED;

                        // if there is any blueProgress, decrement it until empty
                        if (bluProgress > 0) {
                                decrementBluProgress();
                        }


                        // if blu progress is empty, and red progress is not, increment red progress until full.
                        else if (redProgress < 255) {
                                incrementRedProgress();
                        }
                }







                // if blue button is held, do stuff.
                else if (team1Button.wasPressed()) {

                        lastCappingTeam = BLU;

                        // if redProgress exists, decrement until empty
                        if (redProgress > 0) {
                                decrementRedProgress();
                        }

                        // if bluProgress is less than full, increment blu progress
                        else if (bluProgress < 255) {
                                incrementBluProgress();
                        }
                }



                // if no button is pressed, do stuff
                else {
                        if ((lastControllingTeam == BLU && redProgress < 255) ||
                            (lastCappingTeam == RED && redProgress < 255)
                            ) {
                                decrementRedProgress();
                        }

                        else if ((lastControllingTeam == RED && bluProgress < 255) ||
                                 (lastCappingTeam == BLU && bluProgress < 255)
                                 ) {
                                decrementBluProgress();
                        }



                }


                // reset the phase19 timer every time it runs
                lastPhase19Check = millis();
        }



        // Update NeoPixels
        displayState();

}

/**
 * Sector Control paused phase
 */
void runPhase20() {

}


/**
 * Sector Control dumb mode (minimal local state-- DooM HQ handles it)
 */
void runPhase25() {

        // Blink the button LEDS on and off every second
        if (millis() - lastPhase25Check > 1000) {
                if (isLit) {
                        digitalWrite(button0LEDPin, HIGH);
                        digitalWrite(button1LEDPin, LOW);
                        isLit = 0;
                }
                else {
                        digitalWrite(button0LEDPin, LOW);
                        digitalWrite(button1LEDPin, HIGH);
                        isLit = 1;
                }
                lastPhase25Check = millis();
        }

        // If button is pressed or released, send event to gameserver
        if (team0Button.wasPressed() && team1Button.isReleased()) {
                radioSendPress(RED);
        }
        else if (team1Button.wasPressed() && team0Button.isReleased()) {
                radioSendPress(BLU);
        }
        else if (team0Button.wasReleased() && team1Button.isReleased()) {
                radioSendRelease(RED);
        }
        else if (team1Button.wasReleased() && team0Button.isReleased()) {
                radioSendRelease(BLU);
        }


        /**
         * Listen for orders from Controlpointer/DooM HQ
         */
        xbee.readPacket();
        if (xbee.getResponse().isAvailable()) {
                // we got a response
                if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
                        xbee.getResponse().getZBRxResponse(rx);

                        if (rx.getOption() == ZB_PACKET_ACKNOWLEDGED) {
                        }

                        else {
                        }


                        // if we get a battery request (DC), respond with voltage
                        if (
                                rx.getData(0) == 'D' &&
                                rx.getData(1) == 'C' &&
                                rx.getData(2) == 'X' &&
                                rx.getData(3) == 'D' &&
                                rx.getData(4) == 'C'
                                )
                        {
                                // DCXDC means go to BATT check phase (it returns afterwards)
                                lastPhase = 25;
                                phase = 8; // batt check phase
                        }

                        // if we get a LED state
                        else if (
                                rx.getData(0) == 'D' &&
                                rx.getData(1) == 'C' &&
                                rx.getData(2) == 'X' &&
                                rx.getData(3) == 'L' &&
                                rx.getData(4) == 'E' &&
                                rx.getData(5) == 'D'
                                )
                        {
                                // DCXLED means we are getting a LED pattern command
                                // set the team (6) to percentage (7);
                                // example: rx.getData(7) => 0x06, rx.getData(8) => 0x04;
                                // concat 6 and 4 to make 64 (Dec:100) (100 percent)
                                // uint16_t percentage = (rx.getData(7)<<4) | rx.getData(8);
                                bool teamNumber = rx.getData(6);
                                uint8_t percentage = rx.getData(7);
                                setProgressBar(teamNumber, percentage);
                        }
                }
        }
}



void radioCheckIn() {
        /**
         * Listen for orders from Controlpointer
         */
        xbee.readPacket();
        if (xbee.getResponse().isAvailable()) {
                // we got a response
                if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
                        xbee.getResponse().getZBRxResponse(rx);

                        if (rx.getOption() == ZB_PACKET_ACKNOWLEDGED) {
                        }

                        else {
                        }

                        // interpret the orders.
                        if (
                                rx.getData(0) == 'D' &&
                                rx.getData(1) == 'C' &&
                                rx.getData(2) == 'X' &&
                                rx.getData(3) == 'G' &&
                                rx.getData(4) == 'A'
                                )
                        {
                                // DCXGA means our order is to start a game


                                if (rx.getData(5) == '0') {
                                        // Our order is to start game 0 (Domination)
                                        phase = 4;
                                }

                                else if (rx.getData(5) == '1') {
                                        // Our order is to start game 1 (Controlpoint)
                                        phase = 19;
                                }
                        }

                        // if we get a battery request (DC), respond with voltage
                        else if (
                                rx.getData(0) == 'D' &&
                                rx.getData(1) == 'C' &&
                                rx.getData(2) == 'X' &&
                                rx.getData(3) == 'D' &&
                                rx.getData(4) == 'C'
                                )
                        {
                                // DCXDC means go to BATT check phase
                                phase = 8;
                        }

                        else if (
                                rx.getData(0) == 'D' &&
                                rx.getData(1) == 'C' &&
                                rx.getData(2) == 'X' &&
                                rx.getData(3) == 'S' &&
                                rx.getData(4) == 'B' &&
                                rx.getData(5) == 'Y'
                                )
                        {
                                // DCXSBY means our order is to standby
                                phase = 2;
                        }
                }
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
        team0Button.read();
        team1Button.read();


        if (phase == 0) {
                runPhase0();
        }

        /**
         * HELLO Phase
         */
        else if (phase == 1) {
                runPhase1();
        }

        /**
         * Standby Phase
         */
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

        else if (phase == 8) {
                runPhase8();
        }

        else if (phase == 19) {
                runPhase19();
        }

        else if (phase == 20) {
                runPhase20();
        }

        else if (phase == 25) {
                runPhase25();
        }

}
