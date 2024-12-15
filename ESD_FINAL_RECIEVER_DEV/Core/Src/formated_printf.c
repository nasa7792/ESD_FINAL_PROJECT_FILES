
/* ---------------------------------------------------------------------------------
 * Nalin Saxena
 * ECEN 5613 - Fall 2024 - Prof. McClure
 * University of Colorado Boulder
 * Revised 10/12/24
 * File name : formated_printf.c
 *  --------------------------------------------------------------------------------
 * This file contains simple utility functions to format stdio outputs in different colors
   ---------------------------------------------------------------------------------*/

#include"formated_printf.h"

/* -------------------------------------------------- */
//          FUNCTION DEFINITIONS
/* -------------------------------------------------- */

void print_success(char * str){
    printf("\033[32m%s\033[0m\n \r", str); //green
}


void print_info(char * str){
    printf("\033[33m%s\033[0m\n\r", str); //yellow
}

void print_error(char * str){
    printf("\033[31m%s\033[0m\n \r", str); //red
}

void nrf_payload_print(uint8_t data[], uint32_t len)
{
	int i=0;
	while(len--){
		putchar(data[i]);
		i++;
	}
	printf("\n\r");
}
