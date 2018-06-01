#include "SingleVNH5019MotorShield.h"

// Constructors ////////////////////////////////////////////////////////////////

SingleVNH5019MotorShield::SingleVNH5019MotorShield()
{
  //Pin map
  _INA1 = 2;
  _INB1 = 4;
  _PWM1 = 9;
  _EN1DIAG1 = 6;
  _CS1 = A0;
}

SingleVNH5019MotorShield::SingleVNH5019MotorShield(unsigned char INA1,
                                                   unsigned char INB1,
                                                   unsigned char PWM1,
                                                   unsigned char EN1DIAG1,
                                                   unsigned char CS1)
{
  _INA1 = INA1;
  _INB1 = INB1;
  _PWM1 = PWM1;
  _EN1DIAG1 = EN1DIAG1;
  _CS1 = CS1;
}

// Public Methods //////////////////////////////////////////////////////////////
void SingleVNH5019MotorShield::init()
{
// Define pinMode for the pins and set the frequency for timer1.

  pinMode(_INA1,OUTPUT);
  pinMode(_INB1,OUTPUT);
  pinMode(_PWM1,OUTPUT);
  pinMode(_EN1DIAG1,INPUT);
  pinMode(_CS1,INPUT);

  #ifdef SINGLEVNH5019MOTORSHIELD_TIMER1_AVAILABLE
    if (_PWM1 == _PWM1_TIMER1_PIN)
    {
      // Timer 1 configuration
      // prescaler: clockI/O / 1
      // outputs enabled
      // phase-correct PWM
      // top of 400
      //
      // PWM frequency calculation
      // 16MHz / 1 (prescaler) / 2 (phase-correct) / 400 (top) = 20kHz
      TCCR1A = 0b10100000;
      TCCR1B = 0b00010001;
      ICR1 = 400;
    }
  #endif
}
// Set speed for motor 1, speed is a number betwenn -400 and 400
void SingleVNH5019MotorShield::setM1Speed(int speed)
{
  unsigned char reverse = 0;

  if (speed < 0)
  {
    speed = -speed;  // Make speed a positive quantity
    reverse = 1;  // Preserve the direction
  }
  if (speed > 400)  // Max PWM dutycycle
    speed = 400;

  #ifdef SINGLEVNH5019MOTORSHIELD_TIMER1_AVAILABLE
    if (_PWM1 == _PWM1_TIMER1_PIN)
    {
      OCR1A = speed;
    }
    else
    {
      analogWrite(_PWM1,speed * 51 / 80); // map 400 to 255
    }
  #else
    analogWrite(_PWM1,speed * 51 / 80); // map 400 to 255
  #endif

  if (speed == 0)
  {
    digitalWrite(_INA1,LOW);   // Make the motor coast no
    digitalWrite(_INB1,LOW);   // matter which direction it is spinning.
  }
  else if (reverse)
  {
    digitalWrite(_INA1,LOW);
    digitalWrite(_INB1,HIGH);
  }
  else
  {
    digitalWrite(_INA1,HIGH);
    digitalWrite(_INB1,LOW);
  }
}

// Brake motor 1, brake is a number between 0 and 400
void SingleVNH5019MotorShield::setM1Brake(int brake)
{
  // normalize brake
  if (brake < 0)
  {
    brake = -brake;
  }
  if (brake > 400)  // Max brake
    brake = 400;
  digitalWrite(_INA1, LOW);
  digitalWrite(_INB1, LOW);

  #ifdef SINGLEVNH5019MOTORSHIELD_TIMER1_AVAILABLE
    if (_PWM1 == _PWM1_TIMER1_PIN)
    {
      OCR1A = brake;
    }
    else
    {
      analogWrite(_PWM1,brake * 51 / 80); // map 400 to 255
    }
  #else
    analogWrite(_PWM1,brake * 51 / 80); // map 400 to 255
  #endif
}

// Return motor 1 current value in milliamps.
unsigned int SingleVNH5019MotorShield::getM1CurrentMilliamps()
{
  // 5V / 1024 ADC counts / 144 mV per A = 34 mA per count
  return analogRead(_CS1) * 34;
}

// Return error status for motor 1
unsigned char SingleVNH5019MotorShield::getM1Fault()
{
  return !digitalRead(_EN1DIAG1);
}
