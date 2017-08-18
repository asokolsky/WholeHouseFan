
class Fan
{
  /** the PWM pin where fan is */
  uint8_t m_pinPWN;
  uint8_t m_pinCW;
  uint8_t m_pinCCW;
  
  /** the sensor input pin */
  //const int pinFanSensor = 2;

  /** last PWM value we sent to the fan */
  uint8_t m_pwm = 0;

  
public:

  /** min fan PWM value at which fan still spins */
  const static int pwmMin = 16;
  /** min fan PWM value at which a fan can start */
  const static int pwmStart = 30;
  /** max fan PWM value to use.  Could be <255 if Vfan < VpowerSupply  */
  const static int pwmMax = 255;

  Fan(uint8_t pinPWN, uint8_t pinCW, uint8_t pinCCW) : m_pinPWN(pinPWN), m_pinCW(pinCW), m_pinCCW(pinCCW)
  {
    pinMode(pinPWN, OUTPUT);
    pinMode(pinCW, OUTPUT);
    pinMode(pinCCW, OUTPUT);
    //pinMode(pinFanSensor, INPUT);
    /*
    //---------------------------------------------- Set PWM frequency for D3 & D11 ------------------------------
    TCCR2B = TCCR2B & B11111000 | B00000001;    // set timer 2 divisor to     1 for PWM frequency of 31372.55 Hz
    //TCCR2B = TCCR2B & B11111000 | B00000010;    // set timer 2 divisor to     8 for PWM frequency of  3921.16 Hz
    //TCCR2B = TCCR2B & B11111000 | B00000011;    // set timer 2 divisor to    32 for PWM frequency of   980.39 Hz
    //TCCR2B = TCCR2B & B11111000 | B00000100;    // set timer 2 divisor to    64 for PWM frequency of   490.20 Hz (The DEFAULT)
    //TCCR2B = TCCR2B & B11111000 | B00000101;    // set timer 2 divisor to   128 for PWM frequency of   245.10 Hz
    //TCCR2B = TCCR2B & B11111000 | B00000110;    // set timer 2 divisor to   256 for PWM frequency of   122.55 Hz
    //TCCR2B = TCCR2B & B11111000 | B00000111;    // set timer 2 divisor to  1024 for PWM frequency of    30.64 Hz
    */
    setDirection(true);
  }
  /** direction of fan spinning */
  void setDirection(bool bCW)
  {
    digitalWrite(m_pinCW, bCW ? HIGH : LOW);
    digitalWrite(m_pinCCW, bCW ? LOW : HIGH);
  }
  
  /**  stop the fan */
  void stop()
  {
    analogWrite(m_pinPWN, LOW);
    m_pwm = 0;
    DEBUG_PRINT("Stopping the fan.. analogWrite(pinFan) "); DEBUG_PRINTDEC(LOW); DEBUG_PRINTLN("");
  }
  /** spin the fan, pwm is 0...255 */
  void go(unsigned short int pwm)
  {
    analogWrite(m_pinPWN, pwm);
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


