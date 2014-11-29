/*
  ByteI2CMessenger.cpp - ByteI2CMessenger library
  Copyright (C) 2014 Micky Socaci.  All rights reserved.

  Based on Firmata library.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/


#include "Arduino.h"
// #include "HardwareSerial.h"
#include "Wire.h"
#include "ByteI2CMessenger.h"

extern "C" {
#include <string.h>
#include <stdlib.h>
}


//******************************************************************************
//* Support Functions
//******************************************************************************

void sendValueAsTwo7bitBytes(int value)
{
  Wire.write(value & B01111111); // LSB
  Wire.write(value >> 7 & B01111111); // MSB
}

void startSysex(void)
{
  Wire.write(START_SYSEX);
}

void endSysex(void)
{
  Wire.write(END_SYSEX);
}

//******************************************************************************
//* Constructors
//******************************************************************************

ByteI2CMessengerClass::ByteI2CMessengerClass(void)
{
  systemReset();
}

//******************************************************************************
//* Public Methods
//******************************************************************************

/* begin method for overriding default serial bitrate */
void ByteI2CMessengerClass::begin(int address)
{
     Wire.begin(address);
}


//------------------------------------------------------------------------------
// I2C Receive Handling

int ByteI2CMessengerClass::available(void)
{
  return Wire.available();
}

void ByteI2CMessengerClass::processInput(void)
{
  int inputData = Wire.read(); // this is 'int' to handle -1 when no data
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
// I2C Send Handling



void ByteI2CMessengerClass::sendSysex(byte command, byte bytec, byte* bytev)
{
  byte i;
  startSysex();
  Wire.write(command);
  for(i=0; i<bytec; i++) {
    sendValueAsTwo7bitBytes(bytev[i]);
  }
  endSysex();
}

// Internal Actions/////////////////////////////////////////////////////////////

// generic callbacks
void ByteI2CMessengerClass::attach(byte command, systemResetCallbackFunction newFunction)
{
  switch(command) {
  case SYSTEM_RESET: currentSystemResetCallback = newFunction; break;
  }
}

void ByteI2CMessengerClass::attach(byte command, sysexCallbackFunction newFunction)
{
  currentSysexCallback = newFunction;
}

void ByteI2CMessengerClass::detach(byte command)
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
void ByteI2CMessengerClass::systemReset(void)
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
ByteI2CMessengerClass ByteI2CMessenger;


