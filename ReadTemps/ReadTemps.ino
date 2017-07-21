/**
 *  Read the temperatures in the attic and closet and outside.
 *  Use builtin LED to indicate fan operation desired
 *  
 */
#include <Arduino.h>
#include "Trace.h"
#include "Led.h"
 

//
// nothing to customize below
//
Led g_led;


void setup() 
{
  Serial.begin(115200);

  // LM35 is not going to provide more than 1V output and that @100C
  // switch to internal 1.1V reference
  analogReference(INTERNAL);
  pinMode(pinTemp, INPUT);

  //attachInterrupt(0, onHallSensor, RISING);
  g_tempMin = g_tempMax = readTemperature();

  //g_fan.test();
  g_fan.startUpSequence();
}

void loop() 
{
  // temp in degrees C
  unsigned short int temp = readTemperature();
  if(temp < g_tempMin)
    g_tempMin = temp;
  else if(temp > g_tempMax)
    g_tempMax = temp;
  
  Serial.print("readTemperature() => ");  Serial.println(temp);

  if(temp < tempMin) 
  {
    g_fan.stop();
    g_overheatingLed.turnOff();
  }  
  else if(temp < tempMax)
  {
    // determine the setpoint, choice of type important to avoid overflow
    unsigned int pwmFan = map(temp, tempMin, tempMax, Fan::pwmMin, Fan::pwmMax);
    if(g_fan.getLastPWM() == 0) 
    {
      if(pwmFan < Fan::pwmStart) 
        pwmFan = Fan::pwmStart;
    }
    else
    {
      // instead of spinning at target pwm, lets get there gradually.
      pwmFan = (pwmFan + g_fan.getLastPWM())/2;
    }
    g_fan.go(pwmFan);
    g_overheatingLed.turnOff();
  }
  else
  {
    g_fan.go(Fan::pwmMax);
    g_overheatingLed.turnOn();
  }
  //DEBUG_PRINTLN(g_uiCounter);
  //g_uiCounter = 0;

  dumpStats(millis()); 
  delay(4000);
}


