/**
  ******************************************************************************
  * @file           : dac.h
  * @brief          : Header file for DAC 
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 Clinton Guenther
  * All rights reserved.
  *
  ******************************************************************************
  */

#ifndef INC_DAC_H_
#define INC_DAC_H_

#include "stdbool.h"
#include "stm32f1xx_hal.h"

uint16_t get_dac_data_value (float voltage_setting);



#endif /* INC_TIMER_H_ */
