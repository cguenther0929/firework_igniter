/**
  ******************************************************************************
  * @file           : console.c
  * @brief          : Implementation of command line interface 
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 Clinton Guenther
  * All rights reserved.
  *
  ******************************************************************************
  */

#include "console.h"            //Include file to support console

struct UARTMembers uart;


uint8_t getNumber_u8 ( void ) {
    int number      = 0;
    uint8_t timeout     = 0;

    ResetRxBuffer();
    
    uart.rxchar = '\0';
    while(uart.rxchar == '\0'){}                   // Wait for keyboard input 
    
    while (timeout < 20) {
        if(uart.rxchar == ENTER_KEY) break;

        blockingDelay100ms(1);
        timeout++;
    }

    sscanf(uart.rxbuf,"%d",&number);

    print_string("Number Received: ",0);
    PrintUnsignedDecimal((uint8_t)number, LF);

    ResetRxBuffer();
    return(number);
}


float getNumber_float( void ) {
    float number        = 0;
    uint8_t timeout     = 0;
    
    ResetRxBuffer();
    
    uart.rxchar = '\0';
    while(uart.rxchar == '\0'){}                   // Wait for keyboard input 
    
    while (timeout < 80) {
        
        if(uart.rxchar == ENTER_KEY) break;

        blockingDelay100ms(1);
        timeout++;
    }

    sscanf(uart.rxbuf,"%f.3",&number);
    // print_string("Float Number Enterd: ",LF);
    // print_float(number, LF);
    
    ResetRxBuffer();

    return(number);

}

void MainMenu( void ) {
    /*
     * This routine will be using the same buffer as what's 
     * used for the main application.  ResetRxBuffer() is called
     * in main.c just prior to entering this routine. 
     * 
     * The process for calibrating current is explained below.
     * All sensors are calibrated in this way, thus the following
     * explanation will remain generic. 
     *
     * Because the sensors are extremely linear, a line
     * will be constructed from only two point. Said line will
     * be used to then calculate the slope and intercept. It 
     * is the slope and intercept of the line that are stored 
     * as calibration constants within the EEPROM. The Current
     * value (in uA) is assigned to the Y-axis, as the simple 
     * line equation (y=mx+b) will help determine current from an A2D 
     * reading.  Thus the A2D reading will be inserted into the 
     * aforementioned line equation for the value x.  parameters
     * y and b would have already been determined, thus the result
     * of the equation resembles the amount of current flowing (in uA). 
     * 
     * First, the user is required to inject 
     * precisely some minimum value of current through the
     * sensor.  A reading will be taken from the A2D, and
     * said value stored as (minx).  The user shall use 
     * accurate/precise/calibrated equipment to assure 
     * the amount of current being injected is very 
     * near the target value.  
     * 
     * Then, this step is repeated, but this time at an elevated 
     * amount of current.  A new A2D reading is taken and 
     * associated with value (max). Slope can be determined 
     * since rise/run parameters are now known:
     * (uA Current Value) / (max-minx).  Once slope is 
     * determined, the equation y=mx+b can be rewritten in
     * order to solve for b (b = y-mx).  
     * 
     */

	uint8_t     usr_number_u8       = 0;        // Number user has entered will be stored here
    uint16_t    dac_data_value      = 0;
    float       temp_float          = 0.0;
    
    uart.rxchar = '\0';                  
    ResetTerminal();                            // Clear all the contents on the terminal

    
    blockingDelay10ms(1);
    CursorTopLeft();
    blockingDelay10ms(1);

    while(usr_number_u8 != 99) {
        print_string("1 --- Manipulate DAC output.",LF);
        print_string("2 --- Set fuse current.",LF);
        print_string("3 --- Not implemented.",LF);
        
        print_string("99 -- Exit menu.",LF);  
        InsertLineFeed(2);
        
        print_string("Enter Selection:  ",0);
        usr_number_u8 = getNumber_u8();

       
        switch(usr_number_u8) {
            /* Manipulate DAC Voltage Output */
            case 1:
                print_string("What voltage shall the DAC be set to: ",0);
                temp_float = getNumber_float();
            	dac_data_value = get_dac_data_value (temp_float);
                HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (uint32_t)dac_data_value);ClearScreen();
            break;
            
            /* Set fuse current */
            case 2:
                print_string("What shall the fuse current be (mA): ",0);
                temp_float = getNumber_float();
                temp_float = (float)(temp_float/1000.0*0.5);                  // Multiply by 0.5 to get voltage
                print_string("Required DAC Voltage: ",0);
                print_float(temp_float,LF);

            	dac_data_value = get_dac_data_value (temp_float);
                
                HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (uint32_t)dac_data_value);
            	
            break;

            case 3:
                print_string("Not implemented...",LF);
                
            	
            break;



            case 99:
                ResetTerminal();              //Leaving menu, so clear the screen so not to confuse user
                blockingDelay10ms(1);
                CursorTopLeft();        //Make sure the cursor is in the Top Left position
                blockingDelay10ms(1);
                usr_number_u8 = 99;
            break;

            default:
                usr_number_u8 = 99;
        }   //END Switch(usr_number_u8)
    }  //END while(glbinfo.rxchar != 99)

    print_string("Leaving console.",LF);

}
