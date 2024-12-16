/* ---------------------------------------------------------------------------------
 * Nalin Saxena
 * ECEN 5613 - Fall 2024 - Prof. McClure
 * University of Colorado Boulder
 * Revised 10/12/24
 * File usart.h
 *  --------------------------------------------------------------------------------
 * This file contains usart2 initalization and functions for printing debug information 
 * via stdout on a terminal emulator.
 *
 * This file was developed as part of ESD-Lab3 lab assignment and is taken from Lab3.
   ---------------------------------------------------------------------------------*/

#ifndef SRC_USART_H_
#define SRC_USART_H_
/* -------------------------------------------------- */
//          INCLUDES & DEFINES
/* -------------------------------------------------- */
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
