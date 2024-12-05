/*
 * usart.h
 *
 *  Created on: Oct 30, 2024
 *      Author: Nalins
 *
 *  file contains initalization for usart
 *  usart get and put
 */

#ifndef SRC_USART_H_
#define SRC_USART_H_
#define ALTERNATE_MODE (2)
#define BAUD_9600 0x09C4 // clock frequency by desired baud rate
#include<stm32f411xe.h>


/*
 * Intializes usart configures PA2 and PA3 as rx and tx using
 * afrh and moder Register
 * Parameters:
 * 			 none
 *
 * Returns:
 * 			none
 */
void usart_init();

/* A simple function to display character over serial
 *
 * Parameters:
 * 			char ch-> character to be displayed
 *
 * Returns:
 * 			none
 */
void usart_out(char ch);

/* A simple function to take a user character input over serial
 *
 * Parameters:
 * 			none
 *
 *Returns:
 * 			char
 */
char usart_get();


/* A simple function to display string over serial
 * Repeatedly calls usart_out till null is encountered
 *
 * Parameters:
 * 			char *str-> string to be displayed
 *
 *Returns:
 * 			none
 */
void uart_string_out(char *str);



#endif /* SRC_USART_H_ */
