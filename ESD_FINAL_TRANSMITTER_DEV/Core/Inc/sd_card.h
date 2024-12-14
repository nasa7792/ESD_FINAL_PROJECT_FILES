/* ---------------------------------------------------------------------------------
 * Abhirath Koushik
 * ECEN 5613 - Fall 2024 - Prof. McClure
 * University of Colorado Boulder
 * Revised 12/14/24
 *  --------------------------------------------------------------------------------
 * This file contains function declarations related to SD Card Processing Functions.
   ---------------------------------------------------------------------------------*/

#ifndef INC_SD_CARD_H
#define INC_SD_CARD_H

/* -------------------------------------------------- */
//          INCLUDES & DEFINES
/* -------------------------------------------------- */
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* -------------------------------------------------- */
//          FUNCTION DECLARATIONS
/* -------------------------------------------------- */

/*
 * Function to log the payload information to the SD Card.
 *
 * Parameters:
 * 	log_str : Character Array to store the information to be written to the SD Card
 *
 * Returns:
 * 	None
 */
void process_SD_card(char log_str[]);

#endif /* INC_SD_CARD_H */
