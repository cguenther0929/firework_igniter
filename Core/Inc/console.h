/**
  ******************************************************************************
  * @file           : console.h
  * @brief          : Header file for command line interface 
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 Clinton Guenther
  * All rights reserved.
  *
  ******************************************************************************
  */
#ifndef __CONSOLE_H
#define __CONSOLE_H

#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>      // Mostly needed for sscanf()
#include "uart.h"
#include "main.h"
#include "dac.h"
#include "max14662.h"
#include "ad7888.h"

extern UART_HandleTypeDef huart1;
extern DAC_HandleTypeDef hdac;

// TODO Update all comment BLOCKS!!
// TODO Reference console.c

/**
* @brief Main implementation of command-line interface 
* @param: Void passed in
* @retval None
*/
void MainMenu( void );


/**
* @brief Get unsigned (u8) number from user
* @param: Void passed in
* @retval uint8_t
*/
uint8_t getNumber_u8 ( void );

/**
* @brief Get float number from user
* @param: Void passed in
* @retval User-defined float value
*/
float getNumber_float ( void );

#endif
/* END OF FILE */
