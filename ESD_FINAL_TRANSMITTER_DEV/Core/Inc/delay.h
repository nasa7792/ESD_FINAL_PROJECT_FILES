/* ---------------------------------------------------------------------------------
 * Nalin Saxena
 * ECEN 5613 - Fall 2024 - Prof. McClure
 * University of Colorado Boulder
 * Revised 10/12/24
 * File name : delay.h
 *  --------------------------------------------------------------------------------
 * This file contains function declarations regarding systick configuration and creating 
 * delay functions
   ---------------------------------------------------------------------------------*/
#ifndef DELAY_H_
#define DELAY_H_

#include "stdint.h"

/* -------------------------------------------------- */
//          FUNCTION DECLARATIONS
/* -------------------------------------------------- */

/*
 * the function itnializes systick timer 
 * Parameters:
 * 			    none
 * Returns:
 * 			     none
 */
void delay_init();

/*
 * function returns number of millis in ticks
 * Parameters:
 * 			    none
 * Returns:
 * 			     none
 */
uint64_t millis();
/*
 * this function is used to create a software delay for a specificed number of ticks
 * Parameters:
 * 			    time:number of ticks the delay will be applied for 
 * Returns:
 * 			     none
 */
void delay(uint32_t time);
#endif /* DELAY_H_ */
