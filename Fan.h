
class Fan
{
  /** the PWM pin where fan is */
  uint8_t m_pinPWM;
 
  /** last PWM value we sent to the fan */
  uint8_t m_pwm = 0;

  
public:

  /** min fan PWM value at which fan still spins */
  const static uint8_t pwmMin = 16;
  /** min fan PWM value at which a fan can start */
  const static uint8_t pwmStart = 30;
  /** max fan PWM value to use.  Could be <255 if Vfan < VpowerSupply  */
  const static uint8_t pwmMax = 255;

  Fan(uint8_t pinPWM) : m_pinPWM(pinPWM)
  {
    pinMode(pinPWM, OUTPUT);
    // Set PWM frequency
    
  }
  
  /**  stop the fan */
  void stop()
  {
    analogWrite(m_pinPWM, LOW);
    m_pwm = 0;
    DEBUG_PRINT("Stopping the fan.. analogWrite(pinFan) "); DEBUG_PRINTDEC(LOW); DEBUG_PRINTLN("");
  }
  /** spin the fan, pwm is 0...255 */
  void go(unsigned short int pwm)
  {
    analogWrite(m_pinPWM, pwm);
    DEBUG_PRINT("Spinning fan at analogWrite(pinFan) "); DEBUG_PRINTDEC(pwm); DEBUG_PRINTLN("");
    m_pwm = pwm;
  }
  unsigned short int getLastPWM()
  {
    return m_pwm;
  }

  void startUpSequence()
  {
    stop();
    delay(4*1000);
    DEBUG_PRINTLN("spin the fan at start PWM");
    go(pwmStart);
    delay(4*1000);
    DEBUG_PRINTLN("spin the fan at max RPM");
    go(pwmMax);
    delay(4*1000);
    DEBUG_PRINTLN("spin the fan at min RPM");
    go(pwmMin);
    delay(4*1000);  
  }
  
  /**
   * utility to find the min PWM at which this fan starts
   */  
  void test()
  {
    Serial.println("Starting a test to find a min pwm at which this fan can be started...");
    Serial.println("Note the value when we first FAIL to start the fan");
    for(unsigned short int pwm = 90; pwm > 10; pwm = pwm - 8)
    {
      stop();
      delay(4*1000);
      // try to start the fan
      go(pwm);
      delay(4*1000);
    }   
  }
};


