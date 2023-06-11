/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "timer.h"
#include "uart.h"
#include "console.h"
#include "ad7888.h"
#include "max14662.h"
#include <stdbool.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

typedef struct fuse {
    
  uint16_t  fuse_current_u16;                           // Fuse current value in mA
  
  bool      all_fuses_bad_bool;                              // This flag will be set if all fuses are bad
  bool      fuse_lighting_bool;                         // Flag is set when fuse is lighting
    
} fuse;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define DEFAULT_FUSE_CURRENT_MA     50                  // Default fuse current in mA //TODO need to set this to something reasonable
#define CHECK_VALUE_FUSE_CURRENT_MA 300
#define GOOD_FUSE_MV_THRESHOLD      80                  // Voltage threshold in mA
#define FUSE_100MS_TICKS_TIMEOUT    20                  // Number of 100ms counts before shutting active fuses off

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
//TODO need to comment
void igniter_initialize(fuse * fus);

//TODO need to comment
void ignite_fuse (timing * tim, fuse * fus, uint8_t fuse_number_u8);

/**
 * FUNCTION:  void blockingDelay10ms( uint16_t ticks )
 * --------------------
 * Blocking delay function.  Blocks in increments 
 * of 10ms chunks.  Number of "chunks" depends on 
 * "ticks" value passed to function.  This timing 
 * routine is fairly accurate as it uses a timer
 * w/ interrupts to count.  Timebase must first
 * be configured via SetUp function.    
 *
 * returns: Nothing 
 */
// void blockingDelay10ms( uint16_t ticks );

/**
 * FUNCTION:  void blockingDelay100ms( uint16_t ticks )
 * --------------------
 * Blocking delay function.  Blocks in increments 
 * of 100ms chunks.  Number of "chunks" depends on 
 * "ticks" value passed to function.  This timing 
 * routine is fairly accurate as it uses a timer
 * w/ interrupts to count.  Timebase must first
 * be configured via SetUp function.    
 *
 * returns: Nothing 
 */
// void blockingDelay100ms( uint16_t ticks );


//TODO NEED TO COMMENT
uint16_t get_status_all_fuses(fuse * fus);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define HW_REV_0_Pin GPIO_PIN_13
#define HW_REV_0_GPIO_Port GPIOC
#define HW_REV_1_Pin GPIO_PIN_14
#define HW_REV_1_GPIO_Port GPIOC
#define HW_REV_2_Pin GPIO_PIN_15
#define HW_REV_2_GPIO_Port GPIOC
#define HLTH_LED_Pin GPIO_PIN_0
#define HLTH_LED_GPIO_Port GPIOC
#define PBTN_1_Pin GPIO_PIN_1
#define PBTN_1_GPIO_Port GPIOC
#define pbtn_2_Pin GPIO_PIN_2
#define pbtn_2_GPIO_Port GPIOC
#define EXT_LED_1_Pin GPIO_PIN_4
#define EXT_LED_1_GPIO_Port GPIOC
#define EXT_LED_2_Pin GPIO_PIN_5
#define EXT_LED_2_GPIO_Port GPIOC
#define CH1_8_ANSW_CS_n_Pin GPIO_PIN_12
#define CH1_8_ANSW_CS_n_GPIO_Port GPIOB
#define CH1_8_ADC_CS_n_Pin GPIO_PIN_13
#define CH1_8_ADC_CS_n_GPIO_Port GPIOB
#define CH9_16_ADC_CS_n_Pin GPIO_PIN_14
#define CH9_16_ADC_CS_n_GPIO_Port GPIOB
#define CH9_16_ANSW_CS_n_Pin GPIO_PIN_15
#define CH9_16_ANSW_CS_n_GPIO_Port GPIOB
#define EXT_LED_3_Pin GPIO_PIN_6
#define EXT_LED_3_GPIO_Port GPIOC
#define XB_ON_Pin GPIO_PIN_5
#define XB_ON_GPIO_Port GPIOB
#define XB_RST_n_Pin GPIO_PIN_6
#define XB_RST_n_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
