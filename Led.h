
class Led
{
  /** the overheating led pin - use a built-in one */
  const int pinLed = 13;

public:
  Led()
  {
    pinMode(pinLed, OUTPUT);
  }

  void turnOn()
  {
    digitalWrite(pinLed, HIGH);
  }
  void turnOff()
  {
    digitalWrite(pinLed, LOW);
  }
  
};



