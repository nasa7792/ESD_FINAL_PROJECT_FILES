/*
 * I2C.h
 *
 *  Created on: Nov 28, 2024
 *      Author: Nalin Saxena
 */

#ifndef INC_I2C_DRIVER_H_
#define INC_I2C_DRIVER_H_
#include<stm32f411xe.h>
#include "utilities.h"

void I2C_init_config();
void I2C_START_COMS();
void I2C_STOP_COMS();
void I2C_WRITE_DATA(uint8_t data);
void I2C_SEND_ADDRESS(uint8_t address);
void I2C_READ(uint8_t address_slave, uint8_t *buffer_recv,uint8_t recv_buff_size);


#endif /* INC_I2C_DRIVER_H_ */
