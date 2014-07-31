/*
  ByteSerialMessenger.cpp - ByteSerialMessenger library
  Copyright (C) 2011 Micky Socaci.  All rights reserved.

  Based on Firmata library.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/


#include "WProgram.h"
#include "HardwareSerial.h"
#include "ByteSerialMessenger.h"

extern "C" {
#include <string.h>
#include <stdlib.h>
}


//******************************************************************************
//* Support Functions
//******************************************************************************

void sendValueAsTwo7bitBytes(int value)
{
  Serial.print(value & B01111111, BYTE); // LSB
  Serial.print(value >> 7 & B01111111, BYTE); // MSB
}

void startSysex(void)
{
  Serial.print(START_SYSEX, BYTE);
}

void endSysex(void)
{
  Serial.print(END_SYSEX, BYTE);
}

//******************************************************************************
//* Constructors
//******************************************************************************

ByteSerialMessengerClass::ByteSerialMessengerClass(void)
{
  systemReset();
}

//******************************************************************************
//* Public Methods
//******************************************************************************

/* begin method for overriding default serial bitrate */
void ByteSerialMessengerClass::begin(void)
{
  begin(57600);
}

/* begin method for overriding default serial bitrate */
void ByteSerialMessengerClass::begin(long speed)
{
#if defined(__AVR_ATmega128__)  // Wiring
  Serial.begin((uint32_t)speed);
#else
  Serial.begin(speed);
#endif
}


//------------------------------------------------------------------------------
// Serial Receive Handling

int ByteSerialMessengerClass::available(void)
{
  return Serial.available();
}

void ByteSerialMessengerClass::processInput(void)
{
  int inputData = Serial.read(); // this is 'int' to handle -1 when no data
  int command;

  // TODO make sure it handles -1 properly

  if (parsingSysex)
  {
    if(inputData == END_SYSEX)
    {
      //stop sysex byte
      parsingSysex = false;
      //fire off handler function
      if(currentSysexCallback)
        (*currentSysexCallback)(storedInputData[0], sysexBytesRead - 1, storedInputData + 1);
    }
    else
    {
      //normal data byte - add to buffer
      storedInputData[sysexBytesRead] = inputData;
      sysexBytesRead++;
    }
  }
  else if( (waitForData > 0) && (inputData < 128) )
  {
    waitForData--;
    storedInputData[waitForData] = inputData;
  }
  else
  {
    // remove channel info from command byte if less than 0xF0
    if(inputData < 0xF0)
    {
      command = inputData & 0xF0;
      multiByteChannel = inputData & 0x0F;
    }
    else
    {
      command = inputData;
      // commands in the 0xF* range don't use channel data
    }

    switch (command)
    {
    case START_SYSEX:
      parsingSysex = true;
      sysexBytesRead = 0;
      break;
    case SYSTEM_RESET:
      systemReset();
      break;
    }
  }
}

//------------------------------------------------------------------------------
// Serial Send Handling



void ByteSerialMessengerClass::sendSysex(byte command, byte bytec, byte* bytev)
{
  byte i;
  startSysex();
  Serial.print(command, BYTE);
  for(i=0; i<bytec; i++) {
    sendValueAsTwo7bitBytes(bytev[i]);
  }
  endSysex();
}

// Internal Actions/////////////////////////////////////////////////////////////

// generic callbacks
void ByteSerialMessengerClass::attach(byte command, systemResetCallbackFunction newFunction)
{
  switch(command) {
  case SYSTEM_RESET: currentSystemResetCallback = newFunction; break;
  }
}

void ByteSerialMessengerClass::attach(byte command, sysexCallbackFunction newFunction)
{
  currentSysexCallback = newFunction;
}

void ByteSerialMessengerClass::detach(byte command)
{
  switch(command) {
  case SYSTEM_RESET: currentSystemResetCallback = NULL; break;
  case START_SYSEX: currentSysexCallback = NULL; break;
  default:
    attach(command, (callbackFunction)NULL);
  }
}


//******************************************************************************
//* Private Methods
//******************************************************************************



// resets the system state upon a SYSTEM_RESET message from the host software
void ByteSerialMessengerClass::systemReset(void)
{
  byte i;

  waitForData = 0; // this flag says the next serial input will be data
  executeMultiByteCommand = 0; // execute this after getting multi-byte data
  multiByteChannel = 0; // channel data for multiByteCommands


  for(i=0; i<MAX_DATA_BYTES; i++) {
    storedInputData[i] = 0;
  }

  parsingSysex = false;
  sysexBytesRead = 0;

  if(currentSystemResetCallback)
    (*currentSystemResetCallback)();

}



// =============================================================================
// used for flashing the pin for the version number


// make one instance for the user to use
ByteSerialMessengerClass ByteSerialMessenger;


