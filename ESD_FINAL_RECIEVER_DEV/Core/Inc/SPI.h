/* ---------------------------------------------------------------------------------
 * Nalin Saxena
 * ECEN 5613 - Fall 2024 - Prof. McClure
 * University of Colorado Boulder
 * Revised 10/12/24
 * File name : SPI.h
 *  --------------------------------------------------------------------------------
 * This file contains function declarations related to basic SPI communication
   ---------------------------------------------------------------------------------*/

#ifndef INC_SPI_H_
#define INC_SPI_H_
/* -------------------------------------------------- */
//          INCLUDES & DEFINES
/* -------------------------------------------------- */
#include "utilities.h"
#include <stm32f411xe.h>

/*
 * Function to write multiple bytes to spi1 interface

 * Parameters:
 * 			 uint8_t *data_ptr  : pointer of data buffer
 *           int size           : number of bytes to be written
 * Returns:
 * 			none
 */
void SPI_TX_MULTI(uint8_t *data_ptr, int size);

/*
 * Function to read multiple bytes from spi1 interface

 * Parameters:
 * 			 uint8_t *data_ptr  : pointer to data buffer
 *           int size           : number of bytes to be read
 * Returns:
 * 			none
 */
void SPI_READ_MULTI(uint8_t *data_ptr, int size); // read from nrf?

/*
 * Initializes and configures the SPI1 peripherals

 * Parameters:
 * 			 none
 * Returns:
 * 			none
 */
void SPI_INIT();

#endif /* INC_SPI_H_ */