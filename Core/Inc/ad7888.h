/**
  ******************************************************************************
  * @file           : ad7888.h
  * @brief          : Header file for A2D routines  
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 Clinton Guenther
  * All rights reserved.
  *
  ******************************************************************************
  */

#ifndef INC_AD7888_H_
#define INC_AD7888_H_

#include "stm32f1xx_hal.h"
#include "main.h"
#include "timer.h"
#include "uart.h"

//TODO this file needs a lot of work...

extern SPI_HandleTypeDef hspi1;     //TODO this is good






typedef struct ad7888 {
    
  uint8_t     last_channel_sampled;           // How many multiples of 10ms has passes since last reading
    
  //TODO do we want to remove any of the following?  
//     uint8_t ad4681_buffer[4];       // Each transation will require four bytes (32 bits)
//     uint16_t voltage_sample;
//     uint16_t current_sample;
    
//     float   voltage_f;
//     float   current_f;
//     float   power_f;

//     uint8_t cs_res_index;
//     float   cs_res_f;           // This value will need to be configured by the user
//     float   sense_resistors[3];

//     float   run_time_hr;
//     float   run_time_min;


} ad7888;




/**
 * General AD7888 settings, 
 * like sample rates, and 
 * data resolution.
 */
#define A2D_VOLTAGE_PER_BIT             (float)(0.000610352)
#define CS_HAL_DELAY_uS                 5    


/**
 * AD7888 Command Register 
 * bit offsets.
 */
#define AD7888_ADD2_BIT_OFFSET          0x05            //Bit position 5 of command register
#define AD7888_ADD1_BIT_OFFSET          0x04            //Bit position 4 of command register
#define AD7888_ADD0_BIT_OFFSET          0x03            //Bit position 3 of command register
#define AD7888_REF_BIT_OFFSET           0x02            //Bit position 2 of command register
#define AD7888_PM1_BIT_OFFSET           0x01            //Bit position 1 of command register
#define AD7888_PM0_BIT_OFFSET           0x00            //Bit position 0 of command register

/**
 * AD7888 Interface reference  
 * modes
 */
#define AD7888_USE_ON_CHIP_REFERENCE    0x00            // 0 = Use on-chips reference
#define AD7888_USE_EXTERNAL_REFERENCE   0x01            // 1 = Use external reference


/**
 * AD7888 Power Mode Settings
 */
#define AD7888_NORMAL_POWER             0x00            //PM1 = 0 and PM0 = 0 for normal operation
#define AD7888_FULL_SHUTDOWN            0x01            //PM1 = 0 and PM0 = 1 full shutdown 
#define AD7888_AUTO_SHUTDOWN            0x02            //PM1 = 1 and PM0 = 0 Auto shutdown  


/**
 * FUNCTION: void init_ad7888 (void)
 * --------------------
 * @brief    Initialize the AD461 simultaneously 
 * sampling ADC
 * 
 * @return   Nothing 
 * 
*/
void init_ad7888 (ad7888 * a2d);

float get_voltage (ad7888 * a2d, uint8_t channel);

/**
 * TODO need to comment 
 */
// void get_ad4681_samples( ad4681Data * a2d );


#endif /* INC_AD4681_H_ */
