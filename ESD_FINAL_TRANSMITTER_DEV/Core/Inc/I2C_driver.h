/* ---------------------------------------------------------------------------------
 * Nalin Saxena @nasa7792
 * ECEN 5613 - Fall 2024 - Prof. McClure
 * University of Colorado Boulder
 * Revised 10/12/24
 * File name :I2C_driver.h
 *  --------------------------------------------------------------------------------
 * This file contains function declarations related to basic I2C communication
   ---------------------------------------------------------------------------------*/
#ifndef INC_I2C_DRIVER_H_
#define INC_I2C_DRIVER_H_

/* -------------------------------------------------- */
//          FUNCTION DECLARATIONS
/* -------------------------------------------------- */

#include<stm32f411xe.h>
#include "utilities.h"

/*
 * Initalizes i2c1 peripheral setups clock speed configuration and configures pb8 as scl and pb9 as sda
 *
 * Parameters:
 * 			none
 *
 * Returns:
 * 			none
 */
void I2C_init_config();

/*
 * I2C communication start condition
 *
 * Parameters:
 * 			none
 *
 * Returns:
 * 			none
 */
void I2C_START_COMS();

/*
 * I2C communication stop condition
 *
 * Parameters:
 * 			none
 *
 * Returns:
 * 			none
 */
void I2C_STOP_COMS();

/*
 * writes a byte of data with i2c
 *
 * Parameters:
 * 			uint8_t data: data to be written
 *
 * Returns:
 * 			none
 */
void I2C_WRITE_DATA(uint8_t data);

/*
 * sends address of slave device before read/write transaction
 *
 * Parameters:
 * 			uint8_t address : read/write address of slave address
 *
 * Returns:
 * 			none
 */
void I2C_SEND_ADDRESS(uint8_t address);

/*
 * reads from a slave device
 *
 * Parameters:
 * 			uint8_t address_slave : read address of slave device
 *          uint8_t *buffer_recv: buffer to store read contents
 *          uint8_t recv_buff_size : number of bytes to be read from buffer
 *
 * Returns:
 * 			none
 */
void I2C_READ(uint8_t address_slave, uint8_t *buffer_recv,uint8_t recv_buff_size);


#endif /* INC_I2C_DRIVER_H_ */
