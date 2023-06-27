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

/**
 * @brief Get mV reading from A2D (channels 1-16)
 */
//                         A2D Typedef structure passed in                   
//                              |        One-based channel number: so 1-16 here  
//                              |              |
float get_voltage_mv (ad7888 * a2d, uint8_t channel) {

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
  channel++;  // Increase back to one-based number for remaining processing    
  
  if(channel <= 8){
    HAL_GPIO_WritePin(CH1_8_ADC_CS_n_GPIO_Port, CH1_8_ADC_CS_n_Pin, GPIO_PIN_RESET);  //Drop CS line
  }
  else {
    HAL_GPIO_WritePin(CH9_16_ADC_CS_n_GPIO_Port, CH9_16_ADC_CS_n_Pin, GPIO_PIN_RESET);  //Drop CS line
  }

  HAL_Delay(CS_HAL_DELAY_mS);

  //                          Handle to SPI instance                                              
  //                               |              Pointer to transmit buffer                                                     
  //                               |                   |                   Pointer to receive buffer                                     
  //                               |                   |                        |             Size in bytes                  
  //                               |                   |                        |                  |          Timeout in us   
  //                               |                   |                        |                  |             |
  ret = HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)spi_tx_data, (uint8_t *) spi_rx_data, (uint16_t) 2, (uint32_t) 800);
  HAL_Delay(CS_HAL_DELAY_mS);
  HAL_Delay(CS_HAL_DELAY_mS);
  
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


  HAL_GPIO_WritePin(CH1_8_ADC_CS_n_GPIO_Port, CH1_8_ADC_CS_n_Pin, GPIO_PIN_SET);  //Drop CS line
  HAL_GPIO_WritePin(CH9_16_ADC_CS_n_GPIO_Port, CH9_16_ADC_CS_n_Pin, GPIO_PIN_SET);  //Drop CS line
  
  digital_result = (uint16_t)((spi_rx_data[0] << 8) | (spi_rx_data[1]));

  voltage = (float)(digital_result * A2D_VOLTAGE_PER_BIT * 1000); //Value in mV

  return voltage;
}