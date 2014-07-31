/* ExtendedIO.cpp handler for arduino.
 *
 * Copyright 2014 Micky Socaci. Licenced under the GNU GPL Version 3.
 * Contact: micky@nowlive.ro
 *
 */

#include "WProgram.h"
#include "ExtendedIO.h"

/* Width of pulse to trigger the shift register to read and latch.
*/
#define PULSE_WIDTH_USEC   5

/* Optional delay between shift register reads.
*/
#define POLL_DELAY_MSEC   1

#define CHIPS_PER_CACHE_VAR   4
#define DATA_WIDTH 4 * 8
/*
 * Constructor. 
 *
 * int dataPin         = 12; // Connects to the Q7 pin the 165
 * int clockPin        = 11; // Connects to the Clock pin the 165
 * int clockEnablePin  = 9;  // Connects to Clock Enable pin the 165
 * int ploadPin        = 8;  // Connects to Parallel load pin the 165
 *
*/
ExtendedIO::ExtendedIO(int _dataPin, int _clockPin, int _clockEnablePin, int _ploadPin, int _numberOfChips) {
  // Assign variables.                          
  dataPin = _dataPin;
  clockPin = _clockPin;
  clockEnablePin = _clockEnablePin;
  ploadPin = _ploadPin;
  numberOfChips = _numberOfChips;
                           
  // Initialize each digital pin to either output or input
  // We are commanding the shift register with each pin with the exception of the serial
  // data we get back on the dataPin line.
  pinMode(ploadPin, OUTPUT);
  pinMode(clockEnablePin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, INPUT);

  // Required initial states of these two pins according to the datasheet timing diagram
  digitalWrite(clockPin, HIGH);
  digitalWrite(ploadPin, HIGH);
  
  float CHIPS = numberOfChips;

  cacheSections = ceil( CHIPS / CHIPS_PER_CACHE_VAR );
  
  long test = 0;
  long initialCache[8] = { test, test, test, test, test, test, test, test };
  memcpy(cache, initialCache, sizeof initialCache );
  
  
}

void ExtendedIO::process()
{
     read_shift_regs();
}


int ExtendedIO::IOdigitalRead(int pin)
{
    pin = pin-1;
    int cacheId = ceil( pin / int( DATA_WIDTH ) ) ;
    long cacheVar = cache[cacheId];
    int pointer = pin - ( cacheId * DATA_WIDTH );
    return (cacheVar >> pointer) & 1;
}


void ExtendedIO::read_shift_regs()
{
    long bitVal;
    unsigned long bytesVal = 0;

    /* Trigger a parallel Load to latch the state of the data lines,
    */
    digitalWrite(clockEnablePin, HIGH);
    digitalWrite(ploadPin, LOW);
    delayMicroseconds(PULSE_WIDTH_USEC);
    digitalWrite(ploadPin, HIGH);
    digitalWrite(clockEnablePin, LOW);

    /* Loop to read each bit value from the serial out line
     * of the SN74HC165.
    */
    
    // reverse bit stream so pin 1 is bit 1!
    // foreach group of 4     
    for(int i = cacheSections; i >= 0; i--)
    {
      // i should probably use some buffers and only daisy chain 4 shift registers at a time.. 
      // this is where we should be switching betweem chip arrays!
      // request 8 * 4 bits
      for(int z = DATA_WIDTH; z >= 0; z--)
      {
        bitVal = digitalRead(dataPin);

        // Set the corresponding bit in bytesVal.
        bytesVal |= (bitVal << ((DATA_WIDTH) - z));

        // Pulse the Clock (rising edge shifts the next bit).
        digitalWrite(clockPin, HIGH);
        delayMicroseconds(PULSE_WIDTH_USEC);
        digitalWrite(clockPin, LOW);
      }
      
      cache[i] = bytesVal;
    }
    
    
}

// A function that prints all the 1's and 0's of a byte, so 8 bits +or- 2
void ExtendedIO::printLong(long val)
{
    byte i;
    for(byte i = 0; i < 32; i++)
    {
      Serial.print((val >> i) & 1, BIN); // Magic bit shift, if you care look up the <<, >>, and & operators
    }
    Serial.print("\n"); // Go to the next line, do not collect $200
}

void ExtendedIO::debug()
{
     Serial.print("Debug CacheSections:");
     Serial.print(cacheSections);
     for(int i = 0; i < cacheSections; i++)
     {
          Serial.print(" ByteCacheID[");
          Serial.print(i);
          Serial.print("]: ");
          printLong(cache[i]);
     }
}

void ExtendedIO::printPinStates()
{
     for(int i = 1; i <= numberOfChips*8 ; i++ )
     {
          Serial.print("Pin id:");
          Serial.print(i);
          if(IOdigitalRead(i) == HIGH)
          {
              Serial.println(" is HIGH");
          }
          else
          {
             Serial.println(" is LOW");
          }
      }
}

