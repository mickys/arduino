/* ExtendedIO.h handler for arduino.
 *
 * Copyright 2014 Micky Socaci. Licenced under the GNU GPL Version 3.
 * Contact: micky@nowlive.ro
 *
 */
 
#ifndef ExtendedIO_h
#define ExtendedIO_h

#include "WProgram.h"



class ExtendedIO
{
  public:
    ExtendedIO(int, int, int, int, int);
    void process();
    void debug();
    int IOdigitalRead(int);
    void printLong(long);
    int freeRam();
    void printPinStates();

  private:
    void read_shift_regs();
    int DATA_WIDTH;

    int dataPin;
    int clockPin;
    int clockEnablePin;
    int ploadPin;
    int numberOfChips;

    int numberOfPins;
    // unsigned int pinValues[1];
    long cache[8];
    int cacheSections;
};

#endif

