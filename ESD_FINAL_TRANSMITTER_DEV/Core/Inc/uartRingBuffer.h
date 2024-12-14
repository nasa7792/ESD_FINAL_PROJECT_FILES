/* ---------------------------------------------------------------------------------
 * Abhirath Koushik and Controllers Tech
 * ECEN 5613 - Fall 2024 - Prof. McClure
 * University of Colorado Boulder
 * Revised 12/14/24
 *  --------------------------------------------------------------------------------
 * This file contains function declarations related to Ring Buffer for storing the GPS data.
   ---------------------------------------------------------------------------------*/

#ifndef UARTRINGBUFFER_H_
#define UARTRINGBUFFER_H_

/* -------------------------------------------------- */
//          INCLUDES & DEFINES
/* -------------------------------------------------- */
#include "stm32f4xx_hal.h"
#define UART_BUFFER_SIZE 512


/* -------------------------------------------------- */
//          GLOBALS
/* -------------------------------------------------- */
// This structure is defined by Controllers Tech
typedef struct
{
  unsigned char buffer[UART_BUFFER_SIZE];
  volatile unsigned int head;
  volatile unsigned int tail;
} ring_buffer;


/* -------------------------------------------------- */
//          FUNCTION DEFINITIONS
/* -------------------------------------------------- */

/*
 * Function to initialize the Ring Buffer.
 *
 * Parameters:
 * 	None
 *
 * Returns:
 * 	None
 */
void Ringbuf_init(void);

/*
 * Function to read the data in the RX Buffer and increment the Tail count.
 *
 * Parameters:
 * 	None
 *
 * Returns:
 * 	Character read from the RX Buffer
 */
int Uart_read(void);

/*
 * Function to check if data is available to read from the RX Buffer.
 *
 * Parameters:
 * 	None
 *
 * Returns:
 * 	Number of Unread Bytes that are present in the RX Buffer
 */
int IsDataAvailable(void);

/*
 * Function to peek for data in the RX Buffer without incrementing the Tail Count.
 *
 * Parameters:
 * 	None
 *
 * Returns:
 * 	Character peeked from the RX Buffer
 */
int Uart_peek();

/*
 * Function to copy the data from the RX Buffer into another Buffer for parsing the data.
 * Data, including the entered string is copied through this function.
 *
 * Parameters:
 * 	string            : Target string to match in the RX Buffer
 * 	buffertocopyinto  : Buffer to copy the data from the RX Buffer until the match is found
 *
 * Returns:
 * 	1 : Success
 * 	-1: Failure
 */
int Copy_upto (char *string, char *buffertocopyinto);

/*
 * Function to wait until a particular string is detected in the RX Buffer.
 *
 * Parameters:
 * 	string : Target string to match in the RX Buffer
 *
 * Returns:
 * 	1 : Success
 * 	-1: Failure
 */
int Wait_for (char *string);

/*
 * Function representing the UART Interrupt (ISR) Handler.
 *
 * Parameters:
 * 	huart   : USART HAL Declaration
 *
 * Returns:
 *  None
 */
void Uart_isr (UART_HandleTypeDef *huart);

#endif /* UARTRINGBUFFER_H_ */
