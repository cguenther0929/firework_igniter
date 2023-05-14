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

#include "stm32f1xx_hal.h"
extern UART_HandleTypeDef huart1;

// ToDO Cleanup comment blocks and header def
// TODO see console.c or .h

/* Linefeed and Enter Key */
#define LF                          1           //Define short hand for a line feed
#define CR                          2           //Define short hand for carriage return
#define ENTER_KEY                   0x0D     // ASCII code for enter key


/* Message Buffer Sizes */
#define MAX_RX_BUF_INDEX            32                      // Define array element size 
#define MAX_ELEMENTS                MAX_RX_BUF_INDEX + 1    // Number of elements that can be stored in buffer

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
};

/**
 * FUNCTION: void print_float (float number, uint8_t action)
 * --------------------
 * Leverage built-in sprintf to print
 * a float value to the terminal.  
 *
 * returns: Nothing 
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
 * Function: void PrintUnsignedDecimal (uint16_t number, uint8_t action)
 * --------------------
 * Print an unsigned decimal value
 *
 * returns: Nothing 
 */
void PrintUnsignedDecimal (uint16_t number, uint8_t action);

/**
 * FUNCTION: void print_16b_binary_rep (uint16_t number, uint8_t action)
 * --------------------
 * Print a 16bit number, as binary representation
 *
 * returns: Nothing 
 */
void print_16b_binary_rep (uint16_t number, uint8_t action);

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

#endif /* INC_UART_H_ */
