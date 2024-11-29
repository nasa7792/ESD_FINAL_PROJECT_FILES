/*
 * max_heart_sensor_driver.h
 *
 *  Created on: Nov 28, 2024
 *      Author: Asus
 */

#ifndef INC_MAX_HEART_SENSOR_DRIVER_H_
#define INC_MAX_HEART_SENSOR_DRIVER_H_

#include"I2C_driver.h"

#define MAX30102_BASE_ADDRESS 0x57
#define READ_MASK			  1
#define WRITE_MASK			  0
#define MODE_CONFIG_REG		  0x09



void MAX30102_init();
void MAX30102_WRITE_REGISTER(uint8_t register_Address, uint8_t data);
void MAX30102_READ_REGISTER(uint8_t register_Address, uint8_t *recv_buff, uint8_t recv_size);


#endif /* INC_MAX_HEART_SENSOR_DRIVER_H_ */
