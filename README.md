# Firework Igniter (STM32) 
The source contained within this repository is intended for the firework igniter.  This is not to be confused with the firework controller (PC application side).  

## Architecture
Bare metal -- application written in C.       

## Dependencies
* STM32Cube IDE: v1.6.1
   

## Tagged Versions 
v0.0.1 -- The console is up and working.  Routines for getting unsigned decimal and float numbers from the user are working.  Through the console, the fuse current and DAC voltage can be appropriately set -- DAC output working.  The health LED is being driven appropriately.   

v0.0.2 -- Implementation of UART buffering for the XBEE module has been completed.  The buffering operation is untested, however.  SW modified so that a particular analog switch can be enabled.  Readings can now be taken from the A2D.  A2D readings are very accurate.  

v1.0.0 -- This version has been tested with a PC (running the v1.0.0 python application), in which commands were exchanged over a USB cable.  Fuse check functionality is working well.  This version is suitable for running a show.  

v1.0.1 -- Minor code updates to redirect UART 2 output to the XBee wireless module.  Fixed bug where fuse values higher than 9 (multi-digit) couldn't be ignited.  Successfully tested with wireless controller.  

v1.2.0 -- Software has been updated to support setting or reporting the fuse-current value over the wireless protocol.  These features have been successfully tested against the controller.

v1.3.0 -- Software has been updated so that the fuse status is transmitted as four different bytes opposed to two.  This was needed because bytes like 0xFF are forbidden.   

v.1.3.1 -- Bug was resolved in A2D reporting routine.  First noticed when the fuse status for channel 9 was being improperly reported.  The channel number was being decremented for an operation requiring a zero-based channel value, but the channel number wasn't getting increased back to a one-based value for the remainder of the routine.  The SW version can now be retrieved over the terminal.    