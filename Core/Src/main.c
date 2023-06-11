/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
// struct timing       time;
struct UARTMembers  uart; 
ad7888              a2d;    
fuse                fus;
timing              tim;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

DAC_HandleTypeDef hdac;

I2C_HandleTypeDef hi2c2;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim6;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t rx_char;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_DAC_Init(void);
static void MX_I2C2_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM6_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */


  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  uart.errorflag      = false;
  uart.validmsg       = false;
  uart.msg_state      = STATESTART;
  uart.len_verify     = 0;                        // Initialize length verify counter to 0
  uart.producer_index = 0;                        // Initialize consumer index
  uart.consumer_index = 0;                        // Initialize producer index
  uart.inmenu         = false;                    // Will not start out in console menu

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_DAC_Init();
  MX_I2C2_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */

  init_ad7888 (&a2d); 
  init_timer (&tim);
  igniter_initialize(&fus);

  HAL_GPIO_WritePin(EXT_LED_1_GPIO_Port, EXT_LED_1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(EXT_LED_2_GPIO_Port,EXT_LED_2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(EXT_LED_3_GPIO_Port,EXT_LED_3_Pin, GPIO_PIN_RESET);

  // Verify the XBEE module is awake and not in reset
  HAL_GPIO_WritePin(XB_ON_GPIO_Port,XB_ON_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(XB_RST_n_GPIO_Port,XB_RST_n_Pin, GPIO_PIN_SET);


  // Must use Base Start IT if using interrupts
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_DAC_Start(&hdac, DAC_CHANNEL_1);

  HAL_UART_Receive_IT(&huart1, &uart.rxchar, 1);  // UART to console interface
  HAL_UART_Receive_IT(&huart2, &uart.rxchar, 1);  // UART to XBEE interface
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /**
     * Message handling
     */
    if (uart.byte_counter >= MAX_RX_BUF_INDEX) {
      ResetRxBuffer();                        //Something went wrong, reset the RX buffer.
    }
    else if(uart.consumer_index != uart.producer_index) {             //We have unprocessed data when indices do not agree
      HandleByte();
    }

    if(uart.validmsg == true) {     //A valid message confirmed in buffer
        uart.validmsg = false;      //Avoid diving into ProcessMessage for no reason
        ProcessMessage();
    }

    if(tim.timer_100ms_running && ((tim.timer_100ms_cntr) >= FUSE_100MS_TICKS_TIMEOUT)) {
      anlg_sw_all_off();
      fus.fuse_lighting_bool = false;
      tim.timer_100ms_running = false;
      tim.timer_100ms_cntr = 0;

    }


	  if(tim.flag_10ms_tick) {
		  tim.flag_10ms_tick = false;
	  }

	  if(tim.flag_100ms_tick) {
	      tim.flag_100ms_tick = false;
        if(fus.fuse_lighting_bool) {
	        HAL_GPIO_TogglePin(EXT_LED_2_GPIO_Port, EXT_LED_2_Pin); // External RED LED
        }
        else {
          HAL_GPIO_WritePin(EXT_LED_2_GPIO_Port,EXT_LED_2_Pin, GPIO_PIN_RESET);
        }
        
        /* If all fuses are bad, illuminate the amber LED */
	      if(fus.all_fuses_bad_bool){
          HAL_GPIO_TogglePin(EXT_LED_3_GPIO_Port, EXT_LED_3_Pin); // External Amber LED
        }
        else {
          HAL_GPIO_WritePin(EXT_LED_3_GPIO_Port, EXT_LED_3_Pin, GPIO_PIN_RESET);
        }
	  
    }

	    if(tim.flag_500ms_tick) {
	      tim.flag_500ms_tick = false;
	      HAL_GPIO_TogglePin(HLTH_LED_GPIO_Port, HLTH_LED_Pin);   // Board LED
	      HAL_GPIO_TogglePin(EXT_LED_1_GPIO_Port, EXT_LED_1_Pin); // External GRN LED
      

	    }


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV2;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.Prediv1Source = RCC_PREDIV1_SOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  RCC_OscInitStruct.PLL2.PLL2State = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure the Systick interrupt time
  */
  __HAL_RCC_PLLI2S_ENABLE();
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */
  /** DAC Initialization
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }
  /** DAC channel OUT1 config
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 72-1;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 10000-1;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 57600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(HLTH_LED_GPIO_Port, HLTH_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, EXT_LED_1_Pin|EXT_LED_2_Pin|EXT_LED_3_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, CH1_8_ANSW_CS_n_Pin|CH1_8_ADC_CS_n_Pin|CH9_16_ADC_CS_n_Pin|CH9_16_ANSW_CS_n_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(XB_RST_n_GPIO_Port, XB_RST_n_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : HW_REV_0_Pin HW_REV_1_Pin HW_REV_2_Pin PBTN_1_Pin
                           pbtn_2_Pin */
  GPIO_InitStruct.Pin = HW_REV_0_Pin|HW_REV_1_Pin|HW_REV_2_Pin|PBTN_1_Pin
                          |pbtn_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : HLTH_LED_Pin EXT_LED_1_Pin EXT_LED_2_Pin EXT_LED_3_Pin */
  GPIO_InitStruct.Pin = HLTH_LED_Pin|EXT_LED_1_Pin|EXT_LED_2_Pin|EXT_LED_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : CH1_8_ANSW_CS_n_Pin CH1_8_ADC_CS_n_Pin CH9_16_ADC_CS_n_Pin CH9_16_ANSW_CS_n_Pin
                           XB_RST_n_Pin */
  GPIO_InitStruct.Pin = CH1_8_ANSW_CS_n_Pin|CH1_8_ADC_CS_n_Pin|CH9_16_ADC_CS_n_Pin|CH9_16_ANSW_CS_n_Pin
                          |XB_RST_n_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : XB_ON_Pin */
  GPIO_InitStruct.Pin = XB_ON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(XB_ON_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/********************************************//**
 *  @brief Initialize timer events 
 ***********************************************/
void init_timer (timing * tim) {
  tim -> led_fast_blink = false;
  tim -> flag_10ms_tick = false;
  tim -> flag_100ms_tick = false;
  tim -> flag_500ms_tick = false;
  tim -> ticks10ms = 0;
  tim -> ticks100ms = 0;
  tim -> ticks500ms = 0;

  tim -> timer_100ms_running = false;
  tim -> timer_100ms_cntr = 0;

}

/********************************************//**
 *  @brief Ignite a fuse  
 ***********************************************/
void ignite_fuse (timing * tim, fuse * fus, uint8_t fuse_number_u8) {
  float temp_float;
  uint16_t dac_data_value;

  /**
   * Start the timer so the fuse driver
   * shuts off after so many
   * mili-seconds
   */
  tim -> timer_100ms_cntr = 0;
  tim -> timer_100ms_running = true;
  fus -> fuse_lighting_bool = true;

  /**
   * Set the DAC value appropriately
   */
  temp_float = (float)(fus -> fuse_current_u16/1000.0*0.5);  
  dac_data_value = get_dac_data_value(temp_float);
  HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (uint32_t)dac_data_value);

  /**
   * Make sure other fuse channels
   * are first turned off 
   */
  anlg_sw_all_off ();

  /**
   * Turn on the appropriate 
   * analog switch
   */
  set_anlg_sw_on(fuse_number_u8);


}

/********************************************//**
 *  @brief Get fuse status
 ***********************************************/
uint16_t get_status_all_fuses(fuse * fus) {
  uint16_t  temp_fuse_current_u16;
  uint16_t  dac_data_value;
  float     temp_float;
  uint16_t  fuse_status_u16 = 0x0000;
  uint8_t   counter_u8;
  bool      temp_all_fuses_bad_b = true;

  /**
   * Retrieve the current fuse current
   * setting so the value can be 
   * restored before leaving this funciton
   */
  temp_fuse_current_u16 = fus -> fuse_current_u16;

  /**
   * Set the fuse current to a low value
   * so as to not set off fireworks while
   * checking the fuse.  Fuse current value
   * is in mA, so must first divide by 1000
   * before multiplying by 0.5 to determine 
   * DAC voltage.
   */
  fus -> fuse_current_u16 = CHECK_VALUE_FUSE_CURRENT_MA;
  temp_float = (float)(fus -> fuse_current_u16/1000.0*0.5);  

  dac_data_value = get_dac_data_value(temp_float);
  HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (uint32_t)dac_data_value);

  /**
   * Cycle through the fuses to see 
   * which are valid and which are not 
   */
  for(counter_u8 = 1; counter_u8 < 17; counter_u8++){
    set_anlg_sw_on (counter_u8);
    temp_float = get_voltage_mv (&a2d, counter_u8);
    
    if(temp_float > GOOD_FUSE_MV_THRESHOLD) {     // Fuse is good
      fuse_status_u16 |= (1 << (counter_u8 - 1));
      if(temp_all_fuses_bad_b) {
        temp_all_fuses_bad_b = false;
      }
    }

    anlg_sw_all_off();
  }

  /**
   * Update all fuses bad status
   */
  fus -> all_fuses_bad_bool = temp_all_fuses_bad_b;


  /**
   * Return the DAC value back to what it was
   * when entering this function 
   */
  fus -> fuse_current_u16 = temp_fuse_current_u16;
  temp_float = (float)(fus -> fuse_current_u16/1000.0*0.5);  

  dac_data_value = get_dac_data_value(temp_float);
  HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (uint32_t)dac_data_value);

  return fuse_status_u16;
  
}

/********************************************//**
 *  @brief Initalize fuse ignition parameters
 ***********************************************/
void igniter_initialize(fuse * fus){
  fus -> fuse_current_u16 = DEFAULT_FUSE_CURRENT_MA;  
  fus -> fuse_lighting_bool = false;
  fus -> all_fuses_bad_bool = false;
}

/********************************************//**
 *  @brief Handle Timer Interrupts 
 ***********************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if(htim == &htim6){
			tim.flag_10ms_tick = true;

		 if(tim.ticks10ms == 9) {
		   tim.ticks10ms = 0;
		   tim.flag_100ms_tick = true;
       if(tim.timer_100ms_running) {
        tim.timer_100ms_cntr++;
       }
       else {
        tim.timer_100ms_cntr = 0;
       }

		   if(tim.ticks100ms == 4) {
			 tim.ticks100ms = 0;
			 tim.flag_500ms_tick = true;

			 if(tim.ticks500ms == 119)										// One minute worth of half seconds
			   tim.ticks500ms = 0;
			 else
			   tim.ticks500ms += 1;
		   }
		   else {
			   tim.ticks100ms += 1;
		   }
		 }
		 else {
		   tim.ticks10ms += 1;
		 }

		}
}

/********************************************//**
 *  @brief Handle UART RX interrupts 
 ***********************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	
  /**
   * Console UART Interface
   * 
   */
  if(huart == &huart1) {

    uart.rxbuf[uart.producer_index] = uart.rxchar;          // Load this byte into rx buffer  
    uart.byte_counter++;                                                   //Increase data counter
    (uart.producer_index >= MAX_RX_BUF_INDEX) ? (uart.producer_index = 0):(uart.producer_index++);       
		HAL_UART_Receive_IT(&huart1, &uart.rxchar, 1);
	}
	
  /**
   * RF UART Interface 
   */
  else if(huart == &huart2) {

    uart.rxbuf[uart.producer_index] = uart.rxchar;          // Load this byte into rx buffer  
    uart.byte_counter++;                                                   //Increase data counter
    (uart.producer_index >= MAX_RX_BUF_INDEX) ? (uart.producer_index = 0):(uart.producer_index++);       
		HAL_UART_Receive_IT(&huart2, &uart.rxchar, 1);
	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

