// include the ExtendedIO Library
#include <ExtendedIO.h>
// include the ExtendedRotaty Library
#include <ExtendedRotary.h>
// include the TLC5940 Library
#include <Tlc5940.h>

// Serial Debug Mode
boolean SerialDebugMode = false;

// set the ExtendedIo 74HC165 pins
#define DATA_PIN          7
#define CLOCK_ENABLE_PIN  6
#define CLOCK_PIN         5
#define PLOAD_PIN         4

// set number of chips
#define NUMBER_OF_CHIPS   4

// init the io object
ExtendedIO io = ExtendedIO(DATA_PIN, CLOCK_PIN, PLOAD_PIN, CLOCK_ENABLE_PIN, NUMBER_OF_CHIPS);


// Define encoders
#define ENCODER_01 0x01
#define ENCODER_02 0x02
#define ENCODER_03 0x03
#define ENCODER_04 0x04
#define ENCODER_05 0x05
#define ENCODER_06 0x06
#define ENCODER_07 0x07
#define ENCODER_08 0x08
#define ENCODER_09 0x09
#define ENCODER_10 0x0A
#define ENCODER_11 0x0B
#define ENCODER_12 0x0C
#define ENCODER_13 0x0D
#define ENCODER_14 0x0E
#define ENCODER_15 0x0F
#define ENCODER_16 0x10

// Set the number of encoders we have
const int ENCODER_COUNT = 10;

// Create an Array that will store the ExtendedEncoders 
ExtendedRotary Encoders[ENCODER_COUNT] = {
	
	// Encoder ID, Left PIN, Right PIN, Button PIN, Output PIN
	ExtendedRotary(ENCODER_01, 2, 3, 4, 1),
	ExtendedRotary(ENCODER_02, 7, 8, 1, 2),
	ExtendedRotary(ENCODER_03, 12, 5, 6, 3),
	ExtendedRotary(ENCODER_04, 9, 10, 11, 4),  
	ExtendedRotary(ENCODER_05, 14, 15, 16, 5), 
	ExtendedRotary(ENCODER_06, 19, 20, 13, 6),  
	ExtendedRotary(ENCODER_07, 24, 17, 18, 7),
	ExtendedRotary(ENCODER_08, 21, 22, 23, 8),
	ExtendedRotary(ENCODER_09, 26, 27, 28, 9),
	ExtendedRotary(ENCODER_10, 31, 32, 25, 10),
};

// Variable used to determine if any output has changed.
// We do not really want to update the TLC unless there are changes.
boolean tlcChanged = false;


void setup() {
	// Initialize the TLC Library for extended output

	Tlc.init();
	// Initialize the Serial output
	Serial.begin(57600);
}

void loop() {
	// Process ExtendedIO Inputs
	io.process();
	
	// Check changes for each encoder we have
	for (int  i = 0; i < ENCODER_COUNT; i++)
	{
		int channel = 0;
		int value = 0;
		
		boolean buttonState = Encoders[i].getButtonState();
		
		// Encoder button has been pressed!
		if(buttonState) {
			// Get Encoder internal counter.
			int cnt = Encoders[i].getCounter();
			
			if(SerialDebugMode) {
				Serial.print( "Button for enc id=" );
				Serial.print( Encoders[i].getId() );
				Serial.print( " with internal count=" );
				Serial.print( cnt );
				Serial.println( "was pressed");
			}
			
			// If internal counter is 0 ( minimum )
			if(cnt == 0) {
				// set the counter to max
				cnt = 50;
			} else {
				// set the counter to min
				cnt = 0;
			}
			// set the counter into the object
			Encoders[i].setCounter(cnt);
			
			// Output Channels start from 0, that's why we substract 1 
			channel = Encoders[i].getOutput() - 1;
			
			// The TLC expects values between 0 and 4095
			// so we have 0-50 * 82
			value = Encoders[i].getCounter() * 82;
		
			// if value is higher than 4095 it will overflow the TLC
			if(value > 4095){ 
				value = 4095; 
			}
			
			tlcChanged = true;
			Tlc.set(channel, value);
			
			if(SerialDebugMode) {
				Serial.print( "output=" );
				Serial.print( channel );
				Serial.print( " value=" );
				Serial.println( value );
			}
		}

		// Process Encoder direction changes
		int result = Encoders[i].doCounter();
		if ( result > -1 ) {
			Serial.println("result!");
			channel = Encoders[i].getOutput() - 1;
			value = Encoders[i].getCounter() * 82 ;
			if(value > 4095){ value = 4095; } 

			Tlc.set(channel, value);
			tlcChanged = true;
		}
	}

	// Send the update only if the TLC has changes
	if(tlcChanged) {
		while (Tlc.update());
		tlcChanged = false;
	}
	
	
	// Print "HIGH" pins
	// io.printPinStates();
	
	// Print the io cached bytes
	// io.debug();
}
