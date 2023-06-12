/*
 * uart.c
 *
 *  Created on: May 12, 2023
 *      Author: clint
 */
#include "uart.h"

struct UARTMembers uart;
fuse         fus;               // Struct for all things fuse related
timing       tim;               // Struct for all things timing related

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

void print_unsigned_decimal (uint16_t number, uint8_t action) {
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
            /* Check that a valid ID byte was received */         
            if(uart.msg_id > 0x00 && uart.msg_id <= 0x06) {
                uart.msg_state = IDRXED;                        //We have received a valid message ID
            }
            else{
                uart.msg_state = STATESTART;                    //Garbage, so start looking for SOF
            }
        break;

        case(IDRXED):                                    //Now we should be looking at the length value
            uart.msg_len = uart.rxbuf[uart.consumer_index];
            IncrementConsumer();
            if(uart.msg_len <= 0x05) {                  //Put some restriction on this so we know data is valid
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
    uint8_t     temp_length_u8      = 0;
    uint8_t     i                   = 0;
    uint8_t     array_size_u8       = 0;
    uint8_t     fuse_number_u8      = 0;
    uint16_t    dac_data_value_u16  = 0x0000;
    uint16_t    fuse_status_u16     = 0x0000;
    uint16_t    tmp_fus_cur_u16    = 0x0000;
    float       temp_float          = 0.0;
    char        txmessage[MAX_TX_ELEMENTS];                      // Used for sending info to the PTE PC
    char        bcd_array[5];

    uart.errorflag = false;                         // Make sure this is reset
    temp_length_u8 = uart.msg_len;                      // Represents number of bytes in data field only

    switch(uart.msg_id){
        case(ID_FUSESTATUS):
            fuse_status_u16 = get_status_all_fuses(&fus);

            memset(txmessage,0xFF,MAX_TX_ELEMENTS);                             //"Null" out the array -- 0xFF is forbidden
			txmessage[0]= TXSOF;
			txmessage[1]= uart.msg_id;
			txmessage[2]= 0x02;                                                 // Length of the message in bytes (not including EOF)
			txmessage[3]= (uint8_t)((fuse_status_u16 >> 8) & 0xFF);
			txmessage[4]= (uint8_t)(fuse_status_u16 & 0xFF);
			txmessage[5]= FRAMEEND;
			xbee_tx(txmessage);
        
        break;

        case(ID_IGNITE_FUSE):
            xbee_send_ack();

            /**
             * From the message 
             * determine which fuse shall be lit 
             */
			fuse_number_u8 = 0;
            while(temp_length_u8 > 0){
                fuse_number_u8 = (uart.rxbuf[uart.data_index] * Pow10LU(temp_length_u8-1)) + fuse_number_u8;        
                (uart.data_index >= MAX_RX_BUF_INDEX) ? (uart.data_index = 0):(uart.data_index++);                        
                temp_length_u8--;
            }
            
            ignite_fuse (&tim, &fus, fuse_number_u8);
        break;

        /**
         * Set the fuse current
         */
        case(ID_SET_FUSE_CURRENT):
            xbee_send_ack();
            /**
             * From the message 
             * determine what the fuse
             * current shall be set to 
             */
			tmp_fus_cur_u16 = 0;
            while(temp_length_u8 > 0){
                tmp_fus_cur_u16 = (uart.rxbuf[uart.data_index] * Pow10LU(temp_length_u8-1)) + tmp_fus_cur_u16;        
                (uart.data_index >= MAX_RX_BUF_INDEX) ? (uart.data_index = 0):(uart.data_index++);                        
                temp_length_u8--;
            }
            fus.fuse_current_u16 = tmp_fus_cur_u16;
        break;

        /**
         * Report current fuse current 
         * setting
         */
        case(ID_GET_FUSE_CURRENT):
            memset(txmessage,0xFF,MAX_TX_ELEMENTS);
            txmessage[0]= TXSOF;
            txmessage[1]= uart.msg_id;
            dec_to_bcd(fus.fuse_current_u16, bcd_array, &array_size_u8);            //Convert speed value to BCD
            txmessage[2]= array_size_u8;
            for(i=0;i<array_size_u8;i++){
                txmessage[3+i] = bcd_array[i];
            }
            txmessage[3 + array_size_u8] = FRAMEEND;
            xbee_tx(txmessage);
        
        break;


        default:
            print_string("Process Message Error.",LF);
    
    }

}

void xbee_send_ack( void ){
    const char ACK[4] = {0xF2,0x00,0xF6,0xFF};     //0xFF is a terminator, and thus is not transmitted
    xbee_tx(ACK);                                 
} /* End of xbee_send_ack */

void IncrementConsumer( void ) {
    uart.consumer_index >= MAX_RX_BUF_INDEX ? (uart.consumer_index = 0):(uart.consumer_index++);                        // Either roll over or increment the "consumer" pointer
    (uart.byte_counter <= 1) ? (uart.byte_counter = 0):(uart.byte_counter--);                                           //Decrement data counter
} /* End of IncrementConsumer */

void xbee_tx(const char *y){
    char c = '\0';
    
    while(*y != 0xFF)
    {
        c = *y;
        HAL_UART_Transmit(&huart2, (uint8_t *) &c, (uint16_t) 0x01, HAL_MAX_DELAY);  
        y++;                           //Increment the pointer memory address
    }
} /* End of xbee_tx */

uint8_t Pow10LU(uint8_t power){
    uint8_t rtnval;               //Created to fix a compiler warning
    if(power > 2){
        uart.errorflag = true;
    }
    else {
        const uint8_t Lookup[4] = {1,10,100,1000};
        return (Lookup[power]);
    }
    return(1);      //Return 1 to prevent compiler warning
}

void dec_to_bcd(uint32_t in_val, char bcd_array[], uint8_t * pSizeBCD){
    uint8_t i = 0;
    uint8_t Digits = 1;               //Used to keep track of how many digits are in the number
    uint32_t test_val = in_val;          //Used to determine how many digits large the BCD value is

    uint8_t Hunthousands = 0x00;       //Used to keep track of the hundred thousands digits
    uint8_t Tenthousands = 0x00;       //Used to keep track of the ten thousands digits
    uint8_t Thousands    = 0x00;       //Used to keep track of the thousands digits
    uint8_t Hundreds     = 0x00;       //Used to keep track of the hundreds digits
    uint8_t Tens         = 0x00;       //Used to keep track of the tens digits
    uint8_t Ones         = 0x00;       //Used to keep track of the ones digits

    while(test_val >= 10){           //Determine number of BCD digits
        test_val = test_val / 10;
        Digits++;                   //Initialized to zero, thus no need to add one after this
    }

    for(i = 0; i < 32; i++){        //Perform "if column greater than 5, then increase by 3" operation
        if(Hunthousands >= 5){
            Hunthousands = (Hunthousands + 3) & 0x0F;
        }
        if(Tenthousands >= 5){
            Tenthousands = (Tenthousands + 3) & 0x0F;
        }
        if(Thousands >= 5){
            Thousands = (Thousands + 3) & 0x0F;
        }
        if(Hundreds >= 5){
            Hundreds = (Hundreds + 3) & 0x0F;
        }
        if(Tens >= 5){
            Tens = (Tens + 3) & 0x0F;
        }
        if(Ones >= 5){
            Ones = (Ones + 3) & 0x0F;
        }

        Hunthousands = (uint8_t)((Hunthousands << 1) & 0x0F);                              //Shift hundred thousands to the left by one bit
        Hunthousands = (uint8_t)((Hunthousands | ((Tenthousands >> 3) & 0x01)) & 0x0F);    //Replace hundred thousands[0] with ten thousands[3]

        Tenthousands = (uint8_t)((Tenthousands << 1) & 0x0F);                              //Shift ten thousands to the left by one bit
        Tenthousands = (uint8_t)((Tenthousands | ((Thousands >> 3) & 0x01)) & 0x0F);       //Replace ten thousands[0] with thousands[3]

        Thousands = (uint8_t)((Thousands << 1) & 0x0F);                                    //Shift thousands to the left by one bit
        Thousands = (uint8_t)((Thousands | ((Hundreds >> 3) & 0x01)) & 0x0F);              //Replace thousands[0] with hundreds[3]

        Hundreds = (uint8_t)((Hundreds << 1) & 0x0F);                                      //Shift hundreds to the left by one bit
        Hundreds = (uint8_t)((Hundreds | ((Tens >> 3) & 0x01)) & 0x0F);                    //Replace hundreds[0] with tens[3]

        Tens = (uint8_t)((Tens << 1) & 0x0F);                                              //Shift tens to the left by one bit
        Tens = (uint8_t)((Tens | ((Ones >> 3) & 0x01)) & 0x0F);                            //Replace tens[0] with ones[3]

        Ones = (uint8_t)((Ones << 1) & 0x0F);                                              //Shift tens to the left by one bit
        Ones = (uint8_t)((Ones | ((in_val >> (31-i)) & 0x01)) & 0x0F);                      //Replace ones[0] with input value[MSB - itterations]

    }

    i = 0;
    if(Digits > 5){                             //Stuff BCD array according based off of the number of digits
        bcd_array[i] = Hunthousands;
        i++;
    }
    if(Digits > 4){
        bcd_array[i] = Tenthousands;
        i++;
    }
    if(Digits > 3){
        bcd_array[i] = Thousands;
        i++;
    }
    if(Digits > 2){
        bcd_array[i] = Hundreds;
        i++;
    }
    if(Digits > 1){
        bcd_array[i] = Tens;
        i++;
    }
    bcd_array[i] = Ones;
    i++;

    *pSizeBCD = i;                  //Point to how many elements are in the BCD array
} /* End of dec_to_bcd */