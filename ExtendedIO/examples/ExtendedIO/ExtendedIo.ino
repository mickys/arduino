#include <ExtendedIO.h>

// Define 74HC165 array PINS

#define DATA_PIN          7
#define CLOCK_ENABLE_PIN  6
#define CLOCK_PIN         5
#define PLOAD_PIN         4

// Number of 74HC165 chips daisy chained
#define NUMBER_OF_CHIPS   4

// Initialize the ExtendedIo object.
ExtendedIO io = ExtendedIO(DATA_PIN, CLOCK_PIN, PLOAD_PIN, CLOCK_ENABLE_PIN, NUMBER_OF_CHIPS);

// Test pin to read
int testPin = 10;

void setup() {
	// enable serial if you want to debug the ExtendedIO.
    Serial.begin(57600);
}

void loop() {
  io.process();
  // io.debug();
  
  
  if( io.IOdigitalRead(testPin) == HIGH ) {
  	// do something if that pin is HIGH
  }
  else {
  	// do something if the pin is LOW
  }
  
}