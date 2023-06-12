/*
 * uart.h
 *
 *  Created on: May 24, 2021
 *      Author: C. Guenther
 */

#ifndef INC_UART_H_
#define INC_UART_H_

// #include "stdint.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "main.h"

#include "stm32f1xx_hal.h"
extern UART_HandleTypeDef   huart1;
extern UART_HandleTypeDef   huart2;
extern DAC_HandleTypeDef    hdac;

// ToDO Cleanup comment blocks and header def
// TODO see console.c or .h
// TODO Cleanup comments down below

/* Linefeed and Enter Key */
#define LF                          1           //Define short hand for a line feed
#define CR                          2           //Define short hand for carriage return
#define ENTER_KEY                   0x0D     // ASCII code for enter key


/* Message Buffer Sizes */
#define MAX_TX_ELEMENTS             16                          // Maximum size of transmit message
#define MAX_RX_BUF_INDEX            32                          // Define array element size 
#define MAX_ELEMENTS                (MAX_RX_BUF_INDEX + 1)      // Number of elements that can be stored in buffer

/* Message IDs */
#define ID_FUSESTATUS               0x01
#define ID_IGNITE_FUSE              0x02
#define ID_SET_FUSE_CURRENT         0x03
#define ID_GET_FUSE_CURRENT         0x04


/* Message Framing */
#define RXSOF                       0xFC        //Start of frame identifier for message from panel to PC
#define TXSOF                       0xF2
#define FRAMEEND                    0xF6        //End of frame identifier

/* Message States */
#define STATESTART                  0x01        // State variable to indicate we are looking for SOF via UART 
#define SOFRXED                     0x02        // State variable to indicate that we have received the start of frame byte  
#define IDRXED                      0x03        // State variable to indicate that we have received the ID byte 
#define LENRXED                     0x04        // State variable to indicate that we have received the length byte 

/*!
 * Structure for UART members
 */
struct UARTMembers {
    char        rxbuf[MAX_ELEMENTS];            // Ring buffer for serial receiver
    char        rxchar;                         // Supported mainly for calibration routine 
    uint8_t     producer_index;                 // Use this to point to where the next received data byte shall go
    uint8_t     consumer_index;                 // Use this as the consumer of data in the buffer
    uint8_t     data_index;                     // Location where message data starts
    uint8_t     data_end;                       // Location where message data ends    
    uint8_t     msg_state;                      // Keep track of what state we are in
    uint8_t     msg_len;                        // Keep track of the message length field
    uint8_t     byte_counter;                   // Additional variable for tracking number of bytes that are needing to be processed
    uint8_t     len_verify;                     // Used to validate length byte received
    uint8_t     msg_id;                         // Store the ID of the message

    bool        errorflag;                      // Currently used to indicate out-of-bounds range request on power 10 lookup table
    bool        validmsg;                       // Flag to mark that a valid message has been received
    bool        inmenu;                         // This flag will indicate if we're in the menu
};


/**
* @brief Print float number 
* @param number: float number to print
* @param action: LF for line feed or 0 for no line feed
* @retval None
*/
void print_float (float number, uint8_t action);

/**
 * FUNCTION: void print_string(const char * s, uint8_t action)
 * --------------------
 * Print a string through USART1
 *
 * returns: Nothing 
 */
void print_string(const char * s, uint8_t action);

/*
 * Function: void print_unsigned_decimal (uint16_t number, uint8_t action)
 * --------------------
 * Print an unsigned decimal value
 *
 * returns: Nothing 
 */
void print_unsigned_decimal (uint16_t number, uint8_t action);

/**
 * FUNCTION: void print_16b_binary_rep (uint16_t number, uint8_t action)
 * --------------------
 * Print a 16bit number, as binary representation
 *
 * returns: Nothing 
 */
void print_16b_binary_rep (uint16_t number, uint8_t action);


//TODO need to comment
void print_8b_binary_rep (uint8_t number, uint8_t action);

/*
 * Function: void InsertLineFeed( uint8_t line_feeds )
 * --------------------
 * Insert line feeds.  The amount 
 * of line feeds transmitted will depend
 * on the value passed in.
 *
 * returns: Nothing
 */
void InsertLineFeed( uint8_t line_feeds );

/**
 * Function: void InsertLineSeparator( void )
 * --------------------
 * @brief Insert a line on CLI
 * 
 * Draw a line on the CLI to 
 * better segregate chunks of operations. 
 *
 * returns: Nothing
 */
void InsertLineSeparator( void );

/*
 * Function: void CursorTopLeft( void )
 * --------------------
 * Place the cursor in the upper-right 
 * corner of the console screen.
 *
 * returns: Nothing
 */
void CursorTopLeft( void );

/*
 * Function: void ClearCursorUp( void )
 * --------------------
 * Screen space above cursor is cleared.  
 *
 * returns: Nothing
 */
void ClearCursorUp( void );

/*
 * Function: void ClearScreen( void )
 * --------------------
 * Clear entire console screen
 *
 * returns: Nothing
 */
void ClearScreen( void );

/*
 * Function: void ResetTerminal( void )
 * --------------------
 * Command sent to reset terminal -- entire
 * screen will be cleared.  
 *
 * returns: Nothing
 */
void ResetTerminal( void );

/*
 * Function: void ClearLine( void )
 * --------------------
 * Clear only the line of the console screen 
 * where the cursor is currently positioned.  
 *
 * returns: Nothing
 */
void ClearLine( void );

/*
 * Function: void ResetRxBuffer( void )
 * --------------------
 * This routine will null out the receive buffer, 
 * but also producer and consumer pointers 
 * will be reset to 0.
 * 
 * returns: Nothing 
 */
void ResetRxBuffer( void );

//TODO need to comment
void IncrementConsumer( void ); 

//TODO need to comment 
void HandleByte( void );

//TODO clean up comment
/*
 * Function: void ProcessMessage( void )
 * --------------------
 * This function will execute upon the HandleByte
 * stat machine determining a full and valid message 
 * has been received.  
 *
 * returns: Nothing 
 */
void ProcessMessage( void ); 

//TODO need to comment
void xbee_send_ack( void );

//TODO need to comment
void xbee_tx(const char *y);

//TODO need to comment
uint8_t Pow10LU(uint8_t power);

//TODO need to comment
void dec_to_bcd(uint32_t in_val, char bcd_array[], uint8_t * pSizeBCD);

#endif /* INC_UART_H_ */
