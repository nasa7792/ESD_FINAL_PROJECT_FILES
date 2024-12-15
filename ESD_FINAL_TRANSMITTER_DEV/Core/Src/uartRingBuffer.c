/* ---------------------------------------------------------------------------------
 * Abhirath Koushik and Controllers Tech
 * ECEN 5613 - Fall 2024 - Prof. McClure
 * University of Colorado Boulder
 * Revised 12/14/24
 *  --------------------------------------------------------------------------------
 * This file contains function declarations related to Ring Buffer for storing the GPS data.
   ---------------------------------------------------------------------------------*/

/* -------------------------------------------------- */
//          INCLUDES & DEFINES
/* -------------------------------------------------- */
#include <string.h>
#include "UartRingbuffer.h"

#define TIMEOUT_DEF 2000  // 500ms timeout for the functions
#define uart &huart1

/* -------------------------------------------------- */
//          GLOBALS
/* -------------------------------------------------- */
extern UART_HandleTypeDef huart1; // Define the HAL UART Handler
uint16_t timeout;

// Defining the Rx and Tx buffers
ring_buffer rx_buffer = { { 0 }, 0, 0};
ring_buffer tx_buffer = { { 0 }, 0, 0};
ring_buffer *_rx_buffer;
ring_buffer *_tx_buffer;

/* -------------------------------------------------- */
//          FUNCTION DEFINITIONS
/* -------------------------------------------------- */

/*
 * Function to initialize the Ring Buffer.
 * This code was originally written by Controllers Tech.
 *
 * Parameters:
 * 	None
 *
 * Returns:
 * 	None
 */
void Ringbuf_init(void)
{
  _rx_buffer = &rx_buffer;
  _tx_buffer = &tx_buffer;

  /* Enable the UART Error Interrupt*/
  __HAL_UART_ENABLE_IT(uart, UART_IT_ERR);

  /* Enable the UART Data Register not empty Interrupt */
  __HAL_UART_ENABLE_IT(uart, UART_IT_RXNE);
}

/*
 * Function to store characters in the ring buffer.
 * This code was originally written by Controllers Tech.
 *
 * Parameters:
 * 	c       : Character to be stored
 * 	buffer  : Pointer to the buffer which stores the characters
 *
 * Returns:
 * 	None
 */
void store_char(unsigned char c, ring_buffer *buffer)
{
  int i = (unsigned int)(buffer->head + 1) % UART_BUFFER_SIZE;
  if(i != buffer->tail) {
    buffer->buffer[buffer->head] = c;
    buffer->head = i;
  }
}

/*
 * Function to check if data is available to read from the RX Buffer.
 * This code was originally written by Controllers Tech.
 *
 * Parameters:
 * 	None
 *
 * Returns:
 * 	Number of Unread Bytes that are present in the RX Buffer
 */
int Uart_read(void)
{
  // if the head isn't ahead of the tail, we don't have any characters
  if(_rx_buffer->head == _rx_buffer->tail)
  {
    return -1;
  }
  else
  {
    unsigned char c = _rx_buffer->buffer[_rx_buffer->tail];
    _rx_buffer->tail = (unsigned int)(_rx_buffer->tail + 1) % UART_BUFFER_SIZE;
    return c;
  }
}

/*
 * Function to check if data is available to read from the RX Buffer.
 * This code was originally written by Controllers Tech.
 *
 * Parameters:
 * 	None
 *
 * Returns:
 * 	Number of Unread Bytes that are present in the RX Buffer
 */
int IsDataAvailable(void)
{
  return (uint16_t)(UART_BUFFER_SIZE + _rx_buffer->head - _rx_buffer->tail) % UART_BUFFER_SIZE;
}

/*
 * Function to peek for data in the RX Buffer without incrementing the Tail Count.
 * This code was originally written by Controllers Tech.
 *
 * Parameters:
 * 	None
 *
 * Returns:
 * 	Character peeked from the RX Buffer
 */
int Uart_peek()
{
  if(_rx_buffer->head == _rx_buffer->tail)
  {
    return -1;
  }
  else
  {
    return _rx_buffer->buffer[_rx_buffer->tail];
  }
}

/*
 * Function to copy the data from the RX Buffer into another Buffer for parsing the data.
 * Data, including the entered string is copied through this function.
 * This code was originally written by Controllers Tech.
 *
 * Parameters:
 * 	string            : Target string to match in the RX Buffer
 * 	buffertocopyinto  : Buffer to copy the data from the RX Buffer until the match is found
 *
 * Returns:
 * 	1 : Success
 * 	-1: Failure
 */
int Copy_upto (char *string, char *buffertocopyinto)
{
	int so_far =0;
	int len = strlen (string);
	int indx = 0;

again:
	while (Uart_peek() != string[so_far])
		{
			buffertocopyinto[indx] = _rx_buffer->buffer[_rx_buffer->tail];
			_rx_buffer->tail = (unsigned int)(_rx_buffer->tail + 1) % UART_BUFFER_SIZE;
			indx++;
			while (!IsDataAvailable());

		}
	while (Uart_peek() == string [so_far])
	{
		so_far++;
		buffertocopyinto[indx++] = Uart_read();
		if (so_far == len) return 1;
		timeout = TIMEOUT_DEF;
		while ((!IsDataAvailable())&&timeout);
		if (timeout == 0) return 0;
	}

	if (so_far != len)
	{
		so_far = 0;
		goto again;
	}

	if (so_far == len) return 1;
	else return 0;
}

/*
 * Function to wait until a particular string is detected in the RX Buffer.
 * This code was originally written by Controllers Tech.
 *
 * Parameters:
 * 	string : Target string to match in the RX Buffer
 *
 * Returns:
 * 	1 : Success
 * 	-1: Failure
 */
int Wait_for (char *string)
{
	int so_far =0;
	int len = strlen (string);

again:
	timeout = TIMEOUT_DEF;
	while ((!IsDataAvailable())&&timeout);  // let's wait for the data to show up
	if (timeout == 0) return 0;
	while (Uart_peek() != string[so_far])  // peek in the rx_buffer to see if we get the string
	{
		if (_rx_buffer->tail != _rx_buffer->head)
		{
			_rx_buffer->tail = (unsigned int)(_rx_buffer->tail + 1) % UART_BUFFER_SIZE;  // increment the tail
		}

		else
		{
			return 0;
		}
	}
	while (Uart_peek() == string [so_far]) // if we got the first letter of the string
	{
		// now we will peek for the other letters too
		so_far++;
		_rx_buffer->tail = (unsigned int)(_rx_buffer->tail + 1) % UART_BUFFER_SIZE;  // increment the tail
		if (so_far == len) return 1;
		timeout = TIMEOUT_DEF;
		while ((!IsDataAvailable())&&timeout);
		if (timeout == 0) return 0;
	}

	if (so_far != len)
	{
		so_far = 0;
		goto again;
	}

	if (so_far == len) return 1;
	else return 0;
}

/*
 * Function representing the UART Interrupt (ISR) Handler.
 * This code was originally written by Controllers Tech.
 *
 * Parameters:
 * 	huart   : USART HAL Declaration
 *
 * Returns:
 *  None
 */
void Uart_isr (UART_HandleTypeDef *huart)
{
	  uint32_t isrflags   = READ_REG(huart->Instance->SR);
	  uint32_t cr1its     = READ_REG(huart->Instance->CR1);

    /* if DR is not empty and the Rx Int is enabled */
    if (((isrflags & USART_SR_RXNE) != RESET) && ((cr1its & USART_CR1_RXNEIE) != RESET))
    {
		huart->Instance->SR;                       /* Read status register */
        unsigned char c = huart->Instance->DR;     /* Read data register */
        store_char (c, _rx_buffer);  // store data in buffer
        return;
    }

    /*If interrupt is caused due to Transmit Data Register Empty */
    if (((isrflags & USART_SR_TXE) != RESET) && ((cr1its & USART_CR1_TXEIE) != RESET))
    {
    	if(tx_buffer.head == tx_buffer.tail)
    	    {
    	      // Buffer empty, so disable interrupts
    	      __HAL_UART_DISABLE_IT(huart, UART_IT_TXE);

    	    }
    	 else
    	    {
    	      // There is more data in the output buffer. Send the next byte
    	      unsigned char c = tx_buffer.buffer[tx_buffer.tail];
    	      tx_buffer.tail = (tx_buffer.tail + 1) % UART_BUFFER_SIZE;
    	      huart->Instance->SR;
    	      huart->Instance->DR = c;
    	    }
    	return;
    }
}
