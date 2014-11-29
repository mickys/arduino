Micky's collection of Arduino libraries
=======
Libraries i've created or modified.


ExtendedIO
=======
A Library that provides support for reading daisy-chained "8 bit" shift registers like the "74HC165" in an Arduino like way ( io.IODigitalRead(PinNumber) ).

Uses bit shifting in order to preserve chip memory.


ExtendedRotary
=======
A Library for Rotary Encoders that uses the ExtendedIO library.

Supports 2 types of encoders:
- 3 pin ( 2 inputs for encoder + 1 vcc )
- 5 pin ( 2 inputs for encoder + 1 input for button + 2 vcc )

Provides Click & Double Click functionality for the 5 pin version.
Has callback's for encoder value change & button events. 
Ex: setting the output value of an analog pin on encoder change ( ex: brightness for LED's )


ByteSerialMessenger
=======
Firmata based Sysex callback firmware.



ByteI2CMessenger
=======
I2C version of the ByteSerialMessenger library.