# Firework Igniter (STM32) 
The source contained within this repository is intended for the firework igniter.  This is not to be confused with the firework controller (PC application side).  

## Architecture
Bare metal -- application written in C.       

## Dependencies
* STM32Cube IDE: v1.6.1
   

## Tagged Versions 
v0.0.1 -- The console is up and working.  Routines for getting unsigned decimal and float numbers from the user are working.  Through the console, the fuse current and DAC voltage can be appropriately set -- DAC output working.  The health LED is being driven appropriately.   

v0.0.2 -- Implementation of UART buffering for the XBEE module has been completed.  The buffering operation is untested, however.  SW modified so that a particular analog switch can be enabled.  Readings can now be taken from the A2D.  A2D readings are very accurate.  