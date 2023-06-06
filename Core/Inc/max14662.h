/**
  ******************************************************************************
  * @file           : max14662.h
  * @brief          : Header file for analog switch routines  
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 Clinton Guenther
  * All rights reserved.
  *
  ******************************************************************************
  */

#ifndef INC_MAX14662_H_
#define INC_MAX14662_H_

#include "stm32f1xx_hal.h"
#include "main.h"
#include "timer.h"
#include "uart.h"

//TODO this file needs a lot of work...

extern SPI_HandleTypeDef hspi1;     //TODO this is good



/**
 * General AD4681 settings, 
 * like sample rates, and 
 * data resolution.
 * Note, data is returned in 
 * twos complement format.
 */
#define CS_HAL_DELAY_ANLGSW_mS             1          


/**
 * FUNCTION: void init_max14662 (void)
 * --------------------
 * @brief    Initialize the AD461 simultaneously 
 * sampling ADC
 * 
 * @return   Nothing 
 * 
*/
void init_max14662 ( void );


void anlg_sw_all_off ( void );

void anlg_sw_all_on ( void );

bool set_anlg_sw_on (uint8_t channel);

/**
 * TODO need to comment 
 */
// void get_ad4681_samples( ad4681Data * a2d );


#endif /* INC_AD4681_H_ */
