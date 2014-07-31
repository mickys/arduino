/*
 * Rotary encoder library for Arduino.
 */

#ifndef rotary_h
#define rotary_h

#include "WProgram.h"

class Rotary
{
  public:
    Rotary(byte, int, int, int);
    // Process pin(s)
    char process();
    int doCounter();
    void setCounter(int);
    void setBrightness();
    void addCounter();
    void subCounter();
    int getCounter();
    int getId();

  private:
    byte id;
    char state;
    int count;
    int br;
    char pin1;
    char pin2;
    char LED_PIN;
    int LED_INCREMENTS;
};

#endif

