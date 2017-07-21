
class Fan
{
  /** the PWM pin where fan is */
  const int pinFan = 3;
  /** the sensor input pin */
  const int pinFanSensor = 2;

  /** last PWM value we sent to the fan */
  unsigned short int m_pwm = 0;

  
public:

  /** min fan PWM value at which fan still spins */
  const static int pwmMin = 16;
  /** min fan PWM value at which a fan can start */
  const static int pwmStart = 30;
  /** max fan PWM value to use.  Could be <255 if Vfan < VpowerSupply  */
  const static int pwmMax = 255;

  Fan() 
  {
    pinMode(pinFan, OUTPUT);
    pinMode(pinFanSensor, INPUT);
    
    //---------------------------------------------- Set PWM frequency for D3 & D11 ------------------------------
    TCCR2B = TCCR2B & B11111000 | B00000001;    // set timer 2 divisor to     1 for PWM frequency of 31372.55 Hz
    //TCCR2B = TCCR2B & B11111000 | B00000010;    // set timer 2 divisor to     8 for PWM frequency of  3921.16 Hz
    //TCCR2B = TCCR2B & B11111000 | B00000011;    // set timer 2 divisor to    32 for PWM frequency of   980.39 Hz
    //TCCR2B = TCCR2B & B11111000 | B00000100;    // set timer 2 divisor to    64 for PWM frequency of   490.20 Hz (The DEFAULT)
    //TCCR2B = TCCR2B & B11111000 | B00000101;    // set timer 2 divisor to   128 for PWM frequency of   245.10 Hz
    //TCCR2B = TCCR2B & B11111000 | B00000110;    // set timer 2 divisor to   256 for PWM frequency of   122.55 Hz
    //TCCR2B = TCCR2B & B11111000 | B00000111;    // set timer 2 divisor to  1024 for PWM frequency of    30.64 Hz
  }
  
  /**  stop the fan */
  void stop()
  {
    analogWrite(pinFan, LOW);
    m_pwm = 0;
    DEBUG_PRINT("Stopping the fan.. analogWrite(pinFan) "); DEBUG_PRINTDEC(LOW); DEBUG_PRINTLN("");
  }
  /** spin the fan, pwm is 0...255 */
  void go(unsigned short int pwm)
  {
    analogWrite(pinFan, pwm);
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


