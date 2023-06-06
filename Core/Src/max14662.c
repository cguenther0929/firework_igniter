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

void init_max14662 ( void ) {

    HAL_StatusTypeDef ret;
    
    /**
     * The configuration 1 register
     * is reset and defaults as  
     * 0x0000, which results in 
     * the following settings:
     * Oversampling Mode = Disabled
     * Oversampling Ratio = Disabled
     * CRC Write = Disabled
     * CRC Read = Disabled
     * Alert Indicator Function = Disabled
     * Resolution = Normal Resolution
     * Reference Select = Internal Reference Selected
     * Power Down Mode = Normal Mode
     *
     * Details of this register can be 
     * read on datasheet p26 of 29.  
     */


    /**
     * The configuration 2 register
     * is reset and defaults as  
     * 0x0000, which results in 
     * the following settings:
     * Conversion results data format = 2-wire output,
     * so data is output on _both_ pins SDOA and SDOB
     * The reset word = 0x00 (no reset)
     *
     * Details of this register can be 
     * read on datasheet p27 of 29.  
     *
     * All reads and writes to device 
     * registers must consist of two 
     * bytes (16 clock cycles)
     */
    // uint16_t data_buffer[1] = {0x00};
    
    // uint8_t spi_data[2] = {0x00};       //Define the SPI data buffer (index 0 to 15)

    // data_buffer[0] |= (uint16_t)((AD4681_WRITE_BIT << AD4681_WR_BIT_OFFSET) |
    //                                 (AD4681_CONFIG2_REG_ADDR << AD4681_ADDR_BIT_OFFSET) |
    //                                 (OUTPUT_ON_SDOA_ONLY << CONVERSION_MODE_BIT_OFFSET)

    // );

    // spi_data[0] = (uint8_t)((data_buffer[0] >> 8) & 0xFF);
    // spi_data[1] = (uint8_t)(data_buffer[0] & 0xFF);

    // HAL_GPIO_WritePin(ADC_SPI1_CSn_GPIO_Port, ADC_SPI1_CSn_Pin, GPIO_PIN_RESET);
    // HAL_Delay(1);

    // ret = HAL_SPI_Transmit(&hspi1, (uint8_t *)spi_data, (uint16_t) 2, (uint32_t) 200);     // Timeout in us
    // if(ret != HAL_OK){
    //     print_string("SPI Transmit Error",LF);
    // }

    // HAL_GPIO_WritePin(ADC_SPI1_CSn_GPIO_Port, ADC_SPI1_CSn_Pin, GPIO_PIN_SET);

    /**
     * Initializations related to
     * the A2D interface
   */
    // a2d -> sense_resistors[0] = 0.022f;
    // a2d -> sense_resistors[1] = 0.091f;
    // a2d -> sense_resistors[1] = 0.82f;
    // a2d -> cs_res_index = 0;
    // a2d -> cs_res_f = a2d -> sense_resistors[a2d -> cs_res_index];        // Set this to a default value
    // a2d -> first_sample = false;
    // a2d -> logging_status = false;

    // a2d -> run_time_hr = 0.0f;
    // a2d -> run_time_min = 0.0f;
    // a2d -> time_us_elapsed = 0;

}

void anlg_sw_all_off ( void ) {
    uint8_t spi_data[1] = {0x00};       //Define the SPI data buffer (index 0 to 15)

    HAL_StatusTypeDef ret;

    /**
     * First clear switches 1-8
     */
    HAL_GPIO_WritePin(CH1_8_ANSW_CS_n_GPIO_Port, CH1_8_ANSW_CS_n_Pin, GPIO_PIN_RESET);
    HAL_Delay(CS_HAL_DELAY_ANLGSW_mS);

    spi_data[0] = 0x00;

    //                                                                        
    //                   Handle to SPI instance                                                     
    //                        |            Pointer to data buffer                                    
    //                        |                 |             Size in bytes                  
    //                        |                 |                  |          Timeout in us   
    //                        |                 |                  |             |
    ret = HAL_SPI_Transmit(&hspi1, (uint8_t *)spi_data, (uint16_t) 1, (uint32_t) 200);     // Timeout in us
    if(ret != HAL_OK){
        print_string("SPI Transmit Error",LF);
    }


    HAL_GPIO_WritePin(CH1_8_ANSW_CS_n_GPIO_Port, CH1_8_ANSW_CS_n_Pin, GPIO_PIN_SET);
    
    /**
     * Now clear switches 9-16
     */
    HAL_GPIO_WritePin(CH9_16_ANSW_CS_n_GPIO_Port, CH9_16_ANSW_CS_n_Pin, GPIO_PIN_RESET);
    HAL_Delay(CS_HAL_DELAY_ANLGSW_mS);

    spi_data[0] = 0x00;

    //                                                                        
    //                   Handle to SPI instance                                                     
    //                        |            Pointer to data buffer                                    
    //                        |                 |             Size in bytes                  
    //                        |                 |                  |          Timeout in us   
    //                        |                 |                  |             |
    ret = HAL_SPI_Transmit(&hspi1, (uint8_t *)spi_data, (uint16_t) 1, (uint32_t) 200);
    if(ret != HAL_OK){
        print_string("SPI Transmit Error",LF);
    }


    HAL_GPIO_WritePin(CH9_16_ANSW_CS_n_GPIO_Port, CH9_16_ANSW_CS_n_Pin, GPIO_PIN_SET);
}

bool set_anlg_sw_on (uint8_t channel) {
    uint8_t spi_data[1] = {0x00};               // Define the SPI data buffer (index 0 to 15)
    uint16_t temp = 0x0000;

    HAL_StatusTypeDef ret;

    if(channel >= 1 && channel <= 8) {
        HAL_GPIO_WritePin(CH1_8_ANSW_CS_n_GPIO_Port, CH1_8_ANSW_CS_n_Pin, GPIO_PIN_RESET);
        HAL_Delay(CS_HAL_DELAY_ANLGSW_mS);

        spi_data[0] = (uint8_t)(0x01 << (channel-1));
        
        //                                                                        
        //                   Handle to SPI instance                                                     
        //                        |            Pointer to data buffer                                    
        //                        |                 |             Size in bytes                  
        //                        |                 |                  |          Timeout in us   
        //                        |                 |                  |             |
        ret = HAL_SPI_Transmit(&hspi1, (uint8_t *)spi_data, (uint16_t) 1, (uint32_t) 200);
        HAL_GPIO_WritePin(CH1_8_ANSW_CS_n_GPIO_Port, CH1_8_ANSW_CS_n_Pin, GPIO_PIN_SET);

        return true;
        
    }

    else if (channel > 8) {
        temp = channel - 8;        // Shift channel values down to 8 LSBs

        HAL_GPIO_WritePin(CH9_16_ANSW_CS_n_GPIO_Port, CH9_16_ANSW_CS_n_Pin, GPIO_PIN_RESET);
        HAL_Delay(CS_HAL_DELAY_ANLGSW_mS);

        spi_data[0] = (uint8_t)(0x01 << (temp-1));
        
        //                                                                        
        //                   Handle to SPI instance                                                     
        //                        |            Pointer to data buffer                                    
        //                        |                 |             Size in bytes                  
        //                        |                 |                  |          Timeout in us   
        //                        |                 |                  |             |
        ret = HAL_SPI_Transmit(&hspi1, (uint8_t *)spi_data, (uint16_t) 1, (uint32_t) 200);
        HAL_GPIO_WritePin(CH9_16_ANSW_CS_n_GPIO_Port, CH9_16_ANSW_CS_n_Pin, GPIO_PIN_SET);

        return true;

    }

    else {
        return false;
    }




}



void anlg_sw_all_on ( void ) {
    uint8_t spi_data[1] = {0x00};       //Define the SPI data buffer (index 0 to 15)

    HAL_StatusTypeDef ret;

    HAL_GPIO_WritePin(CH1_8_ANSW_CS_n_GPIO_Port, CH1_8_ANSW_CS_n_Pin, GPIO_PIN_RESET);
    HAL_Delay(CS_HAL_DELAY_ANLGSW_mS);

    spi_data[0] = 0xFF;

    //                                                                        
    //                   Handle to SPI instance                                                     
    //                        |            Pointer to data buffer                                    
    //                        |                 |             Size in bytes                  
    //                        |                 |                  |          Timeout in us   
    //                        |                 |                  |             |
    ret = HAL_SPI_Transmit(&hspi1, (uint8_t *)spi_data, (uint16_t) 1, (uint32_t) 200);     // Timeout in us
    if(ret != HAL_OK){
        print_string("SPI Transmit Error",LF);
    }


    HAL_GPIO_WritePin(CH1_8_ANSW_CS_n_GPIO_Port, CH1_8_ANSW_CS_n_Pin, GPIO_PIN_SET);
}

