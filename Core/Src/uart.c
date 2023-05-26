/*
 * uart.c
 *
 *  Created on: May 12, 2023
 *      Author: clint
 */
#include "uart.h"

struct UARTMembers uart;

void print_float (float number, uint8_t action) {
    char temp_buffer[8];        //Define the array that will hold the ASCII values
    char c = '\0';

    /* USE SPRINT F TO BUILD THE ARRAY OF ASCII CHARACTERS */
    sprintf(temp_buffer, "%.4f", number);   //f tells the function we want to print a float value

    HAL_UART_Transmit(&huart1, (uint8_t *) temp_buffer, sizeof(temp_buffer), HAL_MAX_DELAY);

    /* CHECK TO SEE IF THE USER WISHES TO CREATE A NEW LINE */
    if(action == LF) {
        c = '\r';  HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
        c = '\n';  HAL_UART_Transmit(&huart1, (uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    }
    else if(action == CR) {
        c = '\r';  HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    }
}

void print_8b_binary_rep (uint8_t number, uint8_t action) {
    uint16_t i;
    uint8_t tx_char;
    char c = '\0';

    for(i = 8; i > 0; i--) {
        tx_char = (uint16_t)(number >> (i - 1));
        tx_char = (uint16_t)(tx_char & 0x01);
        tx_char = (uint16_t)(tx_char + 0x30);         //Turn this into an ASCII value
        c = (char)tx_char;

        HAL_UART_Transmit(&huart1, (uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    }

    /* CHECK TO SEE IF THE USER WISHES TO CREATE A NEW LINE */
    if(action == LF) {
        c = '\r';  HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
        c = '\n';  HAL_UART_Transmit(&huart1, (uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    }
    else if(action == CR) {
        c = '\r';  HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    }
}

void print_16b_binary_rep (uint16_t number, uint8_t action) {
    uint16_t i;
    uint16_t tx_char;
    char c = '\0';

    for(i = 16; i > 0; i--) {
        tx_char = (uint16_t)(number >> (i - 1));
        tx_char = (uint16_t)(tx_char & 0x01);
        tx_char = (uint16_t)(tx_char + 0x30);         //Turn this into an ASCII value
        c = (char)tx_char;

        HAL_UART_Transmit(&huart1, (uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    }

    /* CHECK TO SEE IF THE USER WISHES TO CREATE A NEW LINE */
    if(action == LF) {
        c = '\r';  HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
        c = '\n';  HAL_UART_Transmit(&huart1, (uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    }
    else if(action == CR) {
        c = '\r';  HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    }
}

void print_string(const char * s, uint8_t action) {
    char c = '\0';

    while(*s != '\0') {
        HAL_UART_Transmit(&huart1, (uint8_t *) s, (uint16_t) 0x01, HAL_MAX_DELAY);
        s++;
    }

    /* CHECK TO SEE IF THE USER WISHES TO CREATE A NEW LINE */
    if(action == LF) {
        c = '\r';  HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
        c = '\n';  HAL_UART_Transmit(&huart1, (uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    }
    else if(action == CR) {
        c = '\r';  HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    }

}

void PrintUnsignedDecimal (uint16_t number, uint8_t action) {
    char temphex[5];        //Define the array that will hold the ASCII values
    char c = '\r';
    uint8_t i;                
    uint8_t decimal_count;    //This is how many digits are written

    /* USE SPRINT F TO BUILD THE ARRAY OF ASCII CHARACTERS */
    decimal_count = (uint8_t)(sprintf(temphex, "%u", number)); //u tells the function we want an unsigned decimal number

    for(i = 0; i < decimal_count; i++) {    //Print out the array of ASCII characters.
        // TXREG1 = (temphex[i]);
        HAL_UART_Transmit(&huart1,(uint8_t *) &temphex[i], (uint16_t) 0x01, HAL_MAX_DELAY);
    }

    /* CHECK TO SEE IF THE USER WISHES TO CREATE A NEW LINE */
    if(action == LF) {
        HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
        c = '\n';  HAL_UART_Transmit(&huart1, (uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    }
    else if(action == CR) {
        c = '\r';  HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    }
}

void ClearCursorUp( void ) {
    char c = 0x1B;
    HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    print_string("[1J",0);        //Send the rest of the sequence to clear the screen

}

void ClearScreen( void ) {
    char c = 0x1B;
    HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    print_string("[2J",0);        //Send the rest of the sequence to clear the screen
}

void ClearLine( void ) {
    char c = 0x1B;
    HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    print_string("[2K",0);        //Send the rest of the sequence to clear the screen

}

void CursorTopLeft( void ) {
    char c = 0x1B;
    HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    print_string("[H",0);     //Send the rest of the sequence to clear the screen
}

void ResetTerminal( void ) {
    char c = 0x1B;
    HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    print_string("c",0);      //Send the rest of the sequence to clear the screen
}

void InsertLineFeed( uint8_t line_feeds ) {
    uint8_t i = 0;         //Use this as a counter
    char c = '\r';
    
    for(i = 0; i < line_feeds; i++){
        HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
        c = '\n'; 
        HAL_UART_Transmit(&huart1,(uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);
    }
}

void InsertLineSeparator( void ) {
    print_string("---------------------------------------", LF);
}

void ResetRxBuffer( void ) {

    uart.consumer_index = uart.producer_index = 0;                              //Reset the pointers
    uart.byte_counter = 0;                                            //Reset the data counter
    memset(uart.rxbuf,'\0',MAX_ELEMENTS);                           //Null out the buffer
} /* End of ResetRxBuffer */

void HandleByte( void ) {     

    switch(uart.msg_state) {
        case(STATESTART):                                   //Have yet to receive a valid SOF
            uart.len_verify = 0;                            //Good place to reset the length verify counter
            if(uart.rxbuf[uart.consumer_index] == RXSOF) {
                uart.msg_state = SOFRXED;                   //Advance state if we receive SOF
                IncrementConsumer();
            }
            else if(uart.rxbuf[uart.consumer_index] == 'z') {     //User wishes to run calibration routine
                uart.inmenu = true;                      //Set this flag so that we don't mess up the rxbuffer
                ResetRxBuffer();                            //Reset this because messaging handling didn't occur while calibrating
                MainMenu();                         //Run the calibration routine
                uart.inmenu = false;                     //Clear this now that the sensors have been calibrated
            }
            else {
                IncrementConsumer();                        //Not a valid byte, keep looking
            }
        break;

        case(SOFRXED):                                   //Now we should be looking at the ID value
            uart.msg_id = uart.rxbuf[uart.consumer_index];
            IncrementConsumer();          
            if(uart.msg_id > 0x00 && uart.msg_id <= 0x20) {
                uart.msg_state = IDRXED;                        //We have received a valid message ID
            }
            else{
                uart.msg_state = STATESTART;                    //Garbage, so start looking for SOF
            }
        break;

        case(IDRXED):                                    //Now we should be looking at the length value
            uart.msg_len = uart.rxbuf[uart.consumer_index];
            IncrementConsumer();
            if(uart.msg_len <= 0x06) {                  //Put some restriction on this so we know data is valid
                uart.msg_state = LENRXED;                        //We have received a valid message ID
            }
            else {
                uart.msg_state = STATESTART;                //Garbage, so start looking for SOF.
            }
        break;

        case(LENRXED):                                       //Now we can collect the data
            if((uart.rxbuf[uart.consumer_index] != FRAMEEND) && uart.len_verify == 0) {          //Still collecting data
                uart.data_index = uart.consumer_index;            //Define where data starts in queue
            }
            
            // else if((uart.rxbuf[uart.consumer_index] != FRAMEEND) && uart.len_verify > 0 && (uart.len_verify < uart.msg_len))
            
            
            else if((uart.rxbuf[uart.consumer_index] == FRAMEEND) && (uart.len_verify == uart.msg_len)){   //We have reached the EOF framing byte
                uart.data_end = uart.consumer_index-1;              //Define where data ends in queue
                uart.msg_state = STATESTART;                //Begin looking for SOF
                uart.validmsg = true;                       //Valid message confirmed
            }
            else if((uart.rxbuf[uart.consumer_index] != FRAMEEND) && (uart.len_verify >= uart.msg_len)) {
                uart.msg_state = STATESTART;
            }

            uart.len_verify++;                           //Used to validate the length byte received
            IncrementConsumer();                        //Move pointer
        break;
    }
} /* End of HandleByte */

void ProcessMessage( void ) {
    //TODO this function needs a ton of work 
    print_string("Process Message.",LF);
}

void SendACK( void ){
    const char ACK[4] = {0xF2,0x00,0xF6,0xFF};     //0xFF is a terminator, and thus is not transmitted
    xbee_tx(ACK);                                 //Send ACK to the PTE PC
} /* End of SendACK */

void IncrementConsumer( void ) {
    uart.consumer_index >= MAX_RX_BUF_INDEX ? (uart.consumer_index = 0):(uart.consumer_index++);                        // Either roll over or increment the "consumer" pointer
    (uart.byte_counter <= 1) ? (uart.byte_counter = 0):(uart.byte_counter--);                                           //Decrement data counter
} /* End of IncrementConsumer */

void xbee_tx(const char *y){
    while(*y != 0xFF)
    {
        HAL_UART_Transmit(&huart2, (uint8_t *) &y, (uint16_t) 0x01, HAL_MAX_DELAY);
        y++;                           //Increment the pointer memory address
    }
} /* End of xbee_tx */