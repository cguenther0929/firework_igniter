/**
  ******************************************************************************
  * @file           : ad7888.c
  * @brief          : Implementation of A2D routines 
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 Clinton Guenther
  * All rights reserved.
  *
  ******************************************************************************
  */

#include "ad7888.h"

//TODO this file needs a lot of work

void init_ad7888 (ad7888 * a2d) {
    a2d -> last_channel_sampled = 0xFF;   // Set to invalid channel

}


float get_voltage (ad7888 * a2d, uint8_t channel) {


//                          Array position [0]
//                            | Array position [1]
//                            |   |
  uint8_t spi_tx_data[2] = {0x55,0x00};       // Define the SPI data buffer (index 0 to 15)
  uint8_t spi_rx_data[2] = {0x00,0x00};
  uint16_t digital_result = 0x000;
  float voltage;                        // Convert to voltage before returning

  HAL_StatusTypeDef ret;

  /**
   * Control register data is loaded in
   * on the first eight clocks of the 
   * transaction.  Channel numbers are 
   * 0 based;
   */
  channel--;    
  spi_tx_data[0] = ( 
                  ((channel >> 2 & 0x01) << AD7888_ADD2_BIT_OFFSET) | 
                  ((channel >> 1 & 0x01) << AD7888_ADD1_BIT_OFFSET) |
                  ((channel & 0x01) << AD7888_ADD0_BIT_OFFSET) |
                  ((AD7888_USE_ON_CHIP_REFERENCE) << AD7888_REF_BIT_OFFSET) | 
                  ((AD7888_NORMAL_POWER >> 1 & 0x01) << AD7888_PM1_BIT_OFFSET) |
                  ((AD7888_NORMAL_POWER & 0x01) << AD7888_PM0_BIT_OFFSET)
                  );

  
  // print_string("SPI CONFIG DATA: ",0);
  // print_8b_binary_rep(spi_tx_data[0],LF);


  HAL_GPIO_WritePin(CH1_8_ADC_CS_n_GPIO_Port, CH1_8_ADC_CS_n_Pin, GPIO_PIN_RESET);  //Drop CS line
  HAL_Delay(CS_HAL_DELAY_uS);
  
  
  

  //                          Handle to SPI instance                                              
  //                               |              Pointer to transmit buffer                                                     
  //                               |                   |                   Pointer to receive buffer                                     
  //                               |                   |                        |             Size in bytes                  
  //                               |                   |                        |                  |          Timeout in us   
  //                               |                   |                        |                  |             |
  ret = HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)spi_tx_data, (uint8_t *) spi_rx_data, (uint16_t) 2, (uint32_t) 800);
  HAL_Delay(CS_HAL_DELAY_uS);
  HAL_Delay(CS_HAL_DELAY_uS);
  
  if(ret != HAL_OK){
    print_string("SPI Transmit Error",LF);
  }

  /**
   * If the last channel sampled is greater than 16
   * or if the last channel sampled does not equal 
   * the current sample, the transaction needs 
   * to be sent again. 
   * Note, upon initialization, "last channel sampled"
   * is set to a high value so the algorithm knows 
   * when the very first sample occurs. 
   */
  if((a2d -> last_channel_sampled > 16) || (a2d -> last_channel_sampled != channel)) {

    //                          Handle to SPI instance                                              
    //                               |              Pointer to transmit buffer                                                     
    //                               |                   |                   Pointer to receive buffer                                     
    //                               |                   |                        |             Size in bytes                  
    //                               |                   |                        |                  |          Timeout in us   
    //                               |                   |                        |                  |             |
    ret = HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)spi_tx_data, (uint8_t *) spi_rx_data, (uint16_t) 2, (uint32_t) 800);

    if(ret != HAL_OK){
      print_string("SPI Transmit Error",LF);
    }    

      a2d -> last_channel_sampled = channel;
    } //END IF



  HAL_GPIO_WritePin(CH1_8_ADC_CS_n_GPIO_Port, CH1_8_ADC_CS_n_Pin, GPIO_PIN_SET);


  digital_result = (uint16_t)((spi_rx_data[0] << 8) | (spi_rx_data[1]));

  print_string("The digital result: ", 0);
  print_16b_binary_rep(digital_result, LF);

  voltage = (float)(digital_result * A2D_VOLTAGE_PER_BIT);

  return voltage;
}



// void get_ad4681_samples( ad4681Data * a2d ) {
//     //TODO the following two lines are in until we 
//     //TODO we get some official code put together
//     uint8_t i = 0;

//     for(i=0; i< 250; i++);
    
    // HAL_StatusTypeDef ret;

    /**
     * Get time value to know time 
     * elapsed between samples
     * TODO need to define this
     */
    // if(!a2d -> first_sample) {
    //     a2d -> time_us_elapsed = get_us_counter();
    // }
    
    
    
    /**
     *  Grab A and B samples.
     * In order for low latency p22
     * in datasheet, need to pulse the 
     * CS line low before taking the sample.
     */
    // HAL_GPIO_WritePin(ADC_SPI1_CSn_GPIO_Port, ADC_SPI1_CSn_Pin, GPIO_PIN_RESET);
    // HAL_Delay(1);
    // blocking_us_delay(CS_PULSE_DELAY_uS);      //TODO we want this line back in
    // HAL_GPIO_WritePin(ADC_SPI1_CSn_GPIO_Port, ADC_SPI1_CSn_Pin, GPIO_PIN_SET);
    // HAL_Delay(1);
    // blocking_us_delay(CS_PULSE_DELAY_uS);        //TODO we want this line back in

    /** 
     * Drop the CS line 
     * to extract the data.
     * Extract data, then bring
     * CS line high again.
    */
    // HAL_GPIO_WritePin(ADC_SPI1_CSn_GPIO_Port, ADC_SPI1_CSn_Pin, GPIO_PIN_RESET);
    // HAL_Delay(1);
    
    // ret = HAL_SPI_Receive(&hspi1, (uint8_t *)a2d -> ad4681_buffer, 4, 200);
    // if(ret != HAL_OK){
    //     print_string("SPI receive error",LF);
    // }
    
    // HAL_GPIO_WritePin(ADC_SPI1_CSn_GPIO_Port, ADC_SPI1_CSn_Pin, GPIO_PIN_SET);
    
    /* Parse into voltage vs. current */
    // a2d -> voltage_sample = (uint16_t)((a2d -> ad4681_buffer[2] << 8) | (a2d -> ad4681_buffer[3]));
    // a2d -> current_sample = (uint16_t)((a2d -> ad4681_buffer[1] << 8) | (a2d -> ad4681_buffer[0]));

    /**
     *  Determine if sign bit
     * is set for twos complement
     * start with voltage sample
     */
    // if((uint16_t)(a2d -> voltage_sample >> 15 & 0x01) == 1 ) {   
    //     a2d -> voltage_sample ^= 0xFFFF;      // Invert all bits
    //     a2d -> voltage_sample += 0x01;        // Add one
        
    //     a2d -> voltage_f = (float)(A2D_VOLTAGE_PER_BIT * a2d -> voltage_sample);
    //     a2d -> voltage_f = (float)(a2d -> voltage_f * VOLTAGE_MEAS_GAIN * -1.0);
    // }
    // else {
    //     a2d -> voltage_f = (float)(A2D_VOLTAGE_PER_BIT * a2d -> voltage_sample);
    //     a2d -> voltage_f = (float)(a2d -> voltage_f * VOLTAGE_MEAS_GAIN);
    // }

    /**
     * Determine current 
     * draw value
     */
    // if((uint16_t)(a2d -> current_sample >> 15 & 0x01) == 1 ) {   
    //     a2d -> current_sample ^= 0xFFFF;      // Invert all bits
    //     a2d -> current_sample += 0x01;        // Add one

    //     a2d -> current_f = (float)(a2d -> current_sample * A2D_VOLTAGE_PER_BIT);        // Raw A2D voltage value
    //     a2d -> current_f = (float)(a2d -> current_f / a2d -> cs_res_f * -1.0);                 // Current = Voltage / Resistance 

    // }
    // else {
    //     a2d -> current_f = (float)(a2d -> current_sample * A2D_VOLTAGE_PER_BIT);        // Raw A2D voltage value
    //     a2d -> current_f = (float)(a2d -> current_f / a2d -> cs_res_f);                 // Current = Voltage / Resistance 
    // }

    /**
     * TODO begin test
     * 
     */
    // a2d -> current_f = 1.2;
    // a2d -> voltage_f = 2.2;
    /**
     * TODO end test
     * 
     */

    /**
     * Calculate power 
     * value 
     */
    // a2d -> power_f = (float)(a2d -> current_f * a2d -> voltage_f);

    // a2d -> first_sample = false;

    /**
     * Start us timer
     * so the elapsed time
     * can be calculated the next 
     * time this function is entered
    */
//    start_us_counter();
    

//}



