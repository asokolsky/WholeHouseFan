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
 
/** LM35 temperature sensor is connected to this pin */
const int pinTemp = A1;

/** the temperature in C to start the fan */
const unsigned short int tempMin = 26; // 30;
/** the maximum temperature in C when fan is at 100% */
const unsigned short int tempMax = 35; // 70;

/** Min observed temperature in C */
unsigned short int g_tempMin = 0;
/** Max observed temperature in C */
unsigned short int g_tempMax = 0;

//
// nothing to customize below
//

Fan g_fan(pinPWN, pinCW, pinCCW);
Led g_overheatingLed(pinLed);

/** Counter for sensor fan feedback */
//volatile unsigned long int g_uiCounter = 0;

/** called because fan hall sensor generates interupts */
/*void onHallSensor()
{
  g_uiCounter++;
}*/

/**  
 * get the temperature and convert it to Celsius 
 * read analog LM35 sensor, 
 * presumes you did analogReference(INTERNAL); - more precise but smaller range
 */
unsigned short int readTemperature() 
{
  // first sample seems to fluctuate a lot. Disregard it
  {
    unsigned int intmp = analogRead(pinTemp);
    //Serial.println(intmp);
    delay(60);
  }

  // according to http://www.atmel.com/dyn/resources/prod_documents/doc8003.pdf
  // 11 bit virtual resolution arduino ADC is 10 bit real resolution
  // for 12 bit resolution 16 samples and >> 4 is needed
  unsigned int reading = 0; // accumulate samples here
  for(int i=0; i<=3; i++)
  {
    unsigned int intmp = analogRead(pinTemp);
    reading = reading + intmp;
    //Serial.println(intmp);
    delay(60);
  }
  reading = reading>>2; // averaged over 4 samples
  /*
  unsigned int reading = analogRead(pinTemp);
  Serial.print("analogRead() => ");
  Serial.println(reading);
  */

  // 110 mV is mapped into 1024 steps.  analogReference(INTERNAL) needed
  float tempC = (float)reading * 110 / 1024;
  return (unsigned short)tempC;
}
/**
 * Dump some statistics so that we can see how the controller and environment are doing...
 */
void dumpStats(unsigned long now)
{
  /** how often to dump stats */
  const unsigned long timeStatsDumpPeriod = 30000;
  /** when we dumped stats last */
  static unsigned long g_timeLastStatsDump = 0;
  // the following will handle rollover just fine!
  if((unsigned long)(now - g_timeLastStatsDump) > timeStatsDumpPeriod) 
  {
    g_timeLastStatsDump = now;
    char buf[256];
    sprintf(buf, "Settings: tempMin=%d, tempMax=%d,", (int)tempMin, (int)tempMax);
    Serial.println(buf);
    sprintf(buf, "Observed: g_tempMin=%d, g_tempMax=%d", (int)g_tempMin, (int)g_tempMax);
    Serial.println(buf);
  }
}

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


