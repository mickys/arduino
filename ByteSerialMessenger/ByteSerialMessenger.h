/*
  ByteSerialMessenger.h - ByteSerialMessenger library
  Copyright (C) 2011 Micky Socaci.  All rights reserved.

  Based on Firmata library.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.
*/


#ifndef ByteSerialMessenger_h
#define ByteSerialMessenger_h

#include <WProgram.h>
#include <inttypes.h>

#define START_SYSEX 		0xF0 	// start a MIDI Sysex message
#define END_SYSEX    		0xF7 	// end a MIDI Sysex message
#define SYSTEM_RESET        0xFF // reset from MIDI

#define SET_PIN_MODE        0xF4
#define SET_PIN 			0x32

#define SEND_INT_VAL        0x33

#define COMMAND_NOT_FOUND   0x34

#define MAX_DATA_BYTES   	32   	// max number of data bytes in non-Sysex messages



extern "C" {
// callback function types
    typedef void (*callbackFunction)(byte, int);
    typedef void (*systemResetCallbackFunction)(void);
    typedef void (*sysexCallbackFunction)(byte command, byte argc, byte*argv);
}

class ByteSerialMessengerClass
{
public:
	ByteSerialMessengerClass();
/* Arduino constructors */
    void begin();
    void begin(long);
/* serial receive handling */
    int available(void);
    void processInput(void);
/* serial send handling */
	void sendSysex(byte command, byte device, byte* bytev);
	/* attach & detach callback functions to messages */
    void attach(byte command, callbackFunction newFunction);
    void attach(byte command, systemResetCallbackFunction newFunction);
    void attach(byte command, sysexCallbackFunction newFunction);
    void detach(byte command);

private:
/* input message handling */
    byte waitForData; // this flag says the next serial input will be data
    byte executeMultiByteCommand; // execute this after getting multi-byte data
    byte multiByteChannel; // channel data for multiByteCommands
    byte storedInputData[MAX_DATA_BYTES]; // multi-byte data
/* sysex */
    boolean parsingSysex;
    int sysexBytesRead;
/* callback functions */
    sysexCallbackFunction currentSysexCallback;
    systemResetCallbackFunction currentSystemResetCallback;
/* private functions */
    void systemReset(void);
};

extern ByteSerialMessengerClass ByteSerialMessenger;


/* Hardware Abstraction Layer */
// #include "Boards.h"

#endif /* ByteSerialMessenger_h */

