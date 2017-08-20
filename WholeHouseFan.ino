/**
 *  FAN temperature controller
 *  Use builtin LED to indicate overheating
 *  
 */
#include <Arduino.h>
#include "NanoViews.h"
#include "Fan.h"
#include "Led.h"
#include "PCB.h"
#include "AnalogNavigationKeypad.h"

//
// nothing to customize below
//

Fan g_fan(pinPWN, pinCW, pinCCW);
Led g_overheatingLed(pinLed);


class MyNavKeyPad: public AnalogNavigationKeypad
{
public:  
  /** this test if for a keyboard connected to A0 and A1 */
  MyNavKeyPad() : AnalogNavigationKeypad(pinKeyPad1, pinKeyPad2)
  {
    
  }
  bool onUserInActivity(unsigned long ulNow);
  bool onKeyAutoRepeat(uint8_t vks);
  bool onKeyDown(uint8_t vks);
  bool onLongKeyDown(uint8_t vks);
  bool onKeyUp(uint8_t vks);
};

bool MyNavKeyPad::onUserInActivity(unsigned long ulNow)
{
  DEBUG_PRINT("MyNavKeyPad::onUserInActivity ulNow="); DEBUG_PRINTDEC(ulNow); DEBUG_PRINTLN("");
  return false; 
}

bool MyNavKeyPad::onKeyAutoRepeat(uint8_t vks)
{
  DEBUG_PRINT("MyNavKeyPad::onKeyAutoRepeat vks="); DEBUG_PRNTLN(getKeyNames(vks));
  return false; 
}

bool MyNavKeyPad::onKeyDown(uint8_t vks)
{
  DEBUG_PRINT("MyNavKeyPad::onKeyDown vks="); DEBUG_PRNTLN(getKeyNames(vks));
  return false; 
}

bool MyNavKeyPad::onLongKeyDown(uint8_t vks)
{
  DEBUG_PRINT("MyNavKeyPad::onLongKeyDown vks="); DEBUG_PRNTLN(getKeyNames(vks));
  return false; 
}

bool MyNavKeyPad::onKeyUp(uint8_t vks)
{
  DEBUG_PRINT("MyNavKeyPad::onKeyUp vks="); DEBUG_PRNTLN(getKeyNames(vks));
  return false; 
}

MyNavKeyPad g_kp;


void setup() 
{
  Serial.begin(115200);  
  delay(1000);   
  //while(!Serial)  ; // wait for serial port to connect. Needed for Leonardo only
  DEBUG_PRINTLN("Whole House Fan!");
    
  //g_kp.setup();  


  //g_fan.test();
  g_fan.startUpSequence();
}

void loop() 
{
  unsigned long ulNow = millis();
  g_kp.getAndDispatchKey(ulNow);
}


