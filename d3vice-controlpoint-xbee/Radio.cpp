#if defined(ARDUINO) && ARDUINO >= 100
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include "Radio.h"
#include <XBee.h>
#include "Phase.h"


Radio::Radio(XBee& xbee, XBeeAddress64 gatewayAddress) : _xbee(xbee), _gatewayAddress(gatewayAddress)
{
  _payloadLength = 3;
  ZBTxRequest _zbRequest = ZBTxRequest(_gatewayAddress, _payload, _payloadLength); // address, payload, size
  ZBTxStatusResponse _zbResponse = ZBTxStatusResponse();
  _lastGreetTime = 0;
}

void Radio::update()
{
  // If the d3vice is in HELLO phase, Radio module is up!
  // Query the XBee network to see if a game is in progress.
  // If a game is in progress, syncronize with it's state.


  // @todo The radio module will need to syncronize with the XBee network during other phases.
  //

  // read the packet in the buffer
  _xbee.readPacket();

  if (_xbee.getResponse().isAvailable()) {

    // Get the API ID of the response packet
    // API IDs are human readable descriptions of the packet's purpose
    // Here we are ensuring the response packet is meant as a respoonse to our request
    if (_xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {

      // fill response object
      _xbee.getResponse().getZBTxStatusResponse(_zbResponse);

      // get delivery status (the fifth byte)
      if (_zbResponse.getDeliveryStatus() == SUCCESS) {
        // successful delivery
      }
      else {
        // failed delivery
      }
    }
  }
  else if (_xbee.getResponse().isError()) {
    // there was an error in the response packet
  }
  
}


/**
 * determine if there is a network game currently in progress
 */
bool Radio::isCurrentGame()
{
  // @todo actually do something to determine if there is a game
  return false;
}


/**
 * greet the network
 */
void Radio::greet()
{

  // greet immediately if this is the first time a greet has occured.
  // this will only happen once per run.
  if (_lastGreetTime == 0) {
    _greet();
    _lastGreetTime = millis();
  }
  
  // greet once every 5 seconds
  else if (millis() - _lastGreetTime >= 10000) {
    _greet();
    
    // log this greet occurance
    _lastGreetTime = millis();
  }
}


/**
 * _greet
 * 
 * called by Radio::greet
 * 
 * do not use this method unless you know what you are doing
 */
void Radio::_greet()
{
  // set address to the broadcast address
  _gatewayAddress.setMsb(0x00000000);
  _gatewayAddress.setMsb(0x0000FFFF);
  
  // configure the payload
  _payload[0] = 'I';
  _payload[1] = 'I';
  _payload[2] = 'I';
  
  // send a warm greeting of radiation
  _xbee.send(_zbRequest);
}


