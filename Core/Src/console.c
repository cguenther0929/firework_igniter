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
ad7888       a2d;               // Struct for all things A2D related 
fuse         fus;               // Struct for all things fuse related
timing       tim;


uint8_t getNumber_u8 ( void ) {
    int number      = 0;
    uint8_t timeout     = 0;

    ResetRxBuffer();
    
    uart.rxchar = '\0';
    while(uart.rxchar == '\0'){}                   // Wait for keyboard input 
    
    while (timeout < 20) {
        if(uart.rxchar == ENTER_KEY) break;

        HAL_Delay(100);          // Delay in ms 
        timeout++;
    }

    sscanf(uart.rxbuf,"%d",&number);

    print_string("Number Received: ",0);
    print_unsigned_decimal((uint8_t)number, LF);

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

        HAL_Delay(100);          // Delay in ms
        timeout++;
    }

    sscanf(uart.rxbuf,"%f.3",&number);
    
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

	uint8_t     usr_number_u8           = 0;        // Number user has entered will be stored here
    uint16_t    dac_data_value          = 0;
    uint16_t    temp_fuse_status_u16    = 0;
    uint8_t     temp_u8                 = 0;
    bool        temp_bool               = 0;
    float       temp_float              = 0.0;
    
    uart.rxchar = '\0';                  
    ResetTerminal();                            // Clear all the contents on the terminal

    
    // blockingDelay10ms(1);
    HAL_Delay(1);
    CursorTopLeft();
    HAL_Delay(1);
    // blockingDelay10ms(1);

    while(usr_number_u8 != 99) {
        InsertLineFeed(1);
        InsertLineSeparator();
        print_string("1 --- Manipulate DAC output.",LF);
        print_string("2 --- Set fuse current.",LF);
        print_string("3 --- Turn particular analog sw ON.",LF);
        print_string("4 --- Turn all analog sw OFF.",LF);
        print_string("5 --- Get voltage from A2D.",LF);
        print_string("6 --- Get fuse status.",LF);
        print_string("7 --- Ignite a fuse.",LF);
        InsertLineFeed(1);
        print_string("8 --- Get SW version information.",LF);


        
        print_string("99 -- Exit menu.",LF);  
        InsertLineFeed(2);
        
        print_string("Enter Selection:  ",0);
        usr_number_u8 = getNumber_u8();

       
        switch(usr_number_u8) {
            /* Manipulate DAC Voltage Output */
            case 1:
                InsertLineFeed(1);
                InsertLineSeparator();
                print_string("What voltage shall the DAC be set to: ",0);
                temp_float = getNumber_float();
                InsertLineFeed(1);
                print_string("Setting DAC voltage to: ",0);
                print_float(temp_float,LF);

            	dac_data_value = get_dac_data_value (temp_float);
                HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (uint32_t)dac_data_value);
            break;
            
            /* Set fuse current */
            case 2:
                InsertLineFeed(1);
                InsertLineSeparator();
                print_string("What shall the fuse current be (mA): ",0);
                temp_float = getNumber_float();
                temp_float = (float)(temp_float/1000.0*0.5);                  // Multiply by 0.5 to get voltage
                print_string("Required DAC Voltage: ",0);
                print_float(temp_float,LF);

            	dac_data_value = get_dac_data_value (temp_float);
                
                HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (uint32_t)dac_data_value);
            	
            break;

            case 3:
                InsertLineFeed(1);
                InsertLineSeparator();
                print_string("Which switch would you like to enable:  ",0);
                temp_u8 = getNumber_u8();

                if(set_anlg_sw_on (temp_u8)) {
                    print_string("Successful. Channel should be enabled.",LF);
                }
                else {
                    print_string("Failed to set analog switch ON.",LF);
                }
                
                
            	
            break;
            
            case 4:
                print_string("Turning all analog SW OFF.",LF);
                anlg_sw_all_off();
            	
            break;
            
            case 5:
                InsertLineFeed(1);
                InsertLineSeparator();
                print_string("Which channel of the A2D shall be sampled:  ",0);
                temp_u8 = getNumber_u8();
                
                print_string("User elected to see channel: ",0);
                print_unsigned_decimal(temp_u8,LF);
                temp_float = get_voltage_mv (&a2d, temp_u8);
                print_string("ADC Voltage: ",0);
                print_float(temp_float,LF);
            	
            break;
            
            case 6:
                InsertLineFeed(1);
                InsertLineSeparator();
                temp_fuse_status_u16 = get_status_all_fuses(&fus);
                print_string("Fuse Status U16: ",0);
                print_16b_binary_rep(temp_fuse_status_u16,LF);
            break;
            
            case 7:
                InsertLineFeed(1);
                InsertLineSeparator();
                
                print_string("Enter fuse to be lit: ",0);
                temp_u8 = getNumber_u8();
                
                print_string("User elected to light fuse: ",0);
                print_unsigned_decimal(temp_u8,LF);
                
                ignite_fuse (&tim, &fus, temp_u8);

                while (tim.timer_100ms_cntr < FUSE_100MS_TICKS_TIMEOUT);
                anlg_sw_all_off();

                tim.timer_100ms_running = false;
                tim.timer_100ms_cntr = 0;

            break;

            case 8:
                InsertLineFeed(1);
                InsertLineSeparator();
                
                print_string("SW Version:",0);
                print_string(SW_VER_STR,LF);

            break;

            case 99:
                ResetTerminal();              //Leaving menu, so clear the screen so not to confuse user
                HAL_Delay(1);
                // blockingDelay10ms(1);
                CursorTopLeft();        //Make sure the cursor is in the Top Left position
                HAL_Delay(1);
                // blockingDelay10ms(1);
                usr_number_u8 = 99;
            break;

            default:
                usr_number_u8 = 0;
        }   //END Switch(usr_number_u8)
    }  //END while(glbinfo.rxchar != 99)

    print_string("Leaving console.",LF);

}
