/*
 * Rotary encoder library for Arduino.
 */

#ifndef ExtendedRotary_h
#define ExtendedRotary_h

#include "Arduino.h"
#include "ExtendedIO.h"


extern "C" {
// callback function types
    typedef int (*IoDigitalReadCallbackFunction)(int);
    typedef void (*ButtonCallbackFunction)(int, int);
}


class ExtendedRotary
{
  public:
    ExtendedRotary(byte, int, int, int, int);

    void attach(IoDigitalReadCallbackFunction newFunction);
    void attach(ButtonCallbackFunction newFunction);
    
    // Process pin(s)
    char process();
    int doCounter();
    void setCounter(int);
    void setBrightness();
    void addCounter();
    void subCounter();
    int getCounter();
    int getId();
    void debug();
    void printLong(long);
    boolean getButtonState();
    int getOutput();

  private:
    byte id;
    char state;
    int count;
    int br;
    int pin1;
    int pin2;
    int LED_INCREMENTS;
    int buttonPin;
    int buttonState;
	int buttonLastState;
    int outputPin;
    
/* callback functions */
    IoDigitalReadCallbackFunction currentIoDigitalRead;
    ButtonCallbackFunction currentButtonCallback;
};

#endif

