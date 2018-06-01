#pragma once

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || \
    defined(__AVR_ATmega328PB__) || defined (__AVR_ATmega32U4__)
  #define SINGLEVNH5019MOTORSHIELD_TIMER1_AVAILABLE
#endif

#include <Arduino.h>

class SingleVNH5019MotorShield
{
  public:
    // CONSTRUCTORS
    // Default pin selection.
    SingleVNH5019MotorShield();
    // User-defined pin selection.
    SingleVNH5019MotorShield(unsigned char INA1,
                             unsigned char INB1,
                             unsigned char PWM1,
                             unsigned char EN1DIAG1,
                             unsigned char CS1);

    // PUBLIC METHODS
    void init(); // Initialize TIMER 1, set the PWM to 20kHZ.
    void setM1Speed(int speed); // Set speed for M1.
    void setM1Brake(int brake); // Brake M1.
    unsigned int getM1CurrentMilliamps(); // Get current reading for M1.
    unsigned char getM1Fault(); // Get fault reading from M1.

  private:
    unsigned char _INA1;
    unsigned char _INB1;
    unsigned char _PWM1;
    static const unsigned char _PWM1_TIMER1_PIN = 9;
    unsigned char _EN1DIAG1;
    unsigned char _CS1;

};
