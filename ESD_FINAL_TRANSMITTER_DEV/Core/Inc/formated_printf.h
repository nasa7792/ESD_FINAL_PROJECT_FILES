/* ---------------------------------------------------------------------------------
 * Nalin Saxena
 * ECEN 5613 - Fall 2024 - Prof. McClure
 * University of Colorado Boulder
 * Revised 10/12/24
 * File name : formated_printf.h
 *  --------------------------------------------------------------------------------
 * This file contains simple utility functions to format stdio outputs in different colors
   ---------------------------------------------------------------------------------*/

#ifndef INC_FORMATED_PRINTF_H_
#define INC_FORMATED_PRINTF_H_

#include <stdio.h>

/* -------------------------------------------------- */
//          FUNCTION DECLARATIONS
/* -------------------------------------------------- */

/*
 * prints message in green color, used to print success messages
 *
 * Parameters:
 * 			 char *str : pointer to message
 *
 * Returns:
 * 			none
 */
void print_success(char *str);

/*
 * prints message in yellow used to info  messages
 *
 * Parameters:
 * 			 char *str : pointer to message
 *
 * Returns:
 * 			none
 */
void print_info(char *str);

/*
 * prints message in red used to error  messages
 *
 * Parameters:
 * 			 char *str : pointer to message
 *
 * Returns:
 * 			none
 */
void print_error(char *str);

#endif /* INC_FORMATED_PRINTF_H_ */
