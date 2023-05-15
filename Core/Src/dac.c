/**
  ******************************************************************************
  * @file           : dac.c
  * @brief          : Implementation of DAC module 
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 Clinton Guenther
  * All rights reserved.
  *
  ******************************************************************************
  */

#include "dac.h"

uint16_t get_dac_data_value (float voltage_setting) {
    uint16_t dac_data_value;
    dac_data_value = (uint16_t)(4096/3.3*voltage_setting);
    return dac_data_value;
}
