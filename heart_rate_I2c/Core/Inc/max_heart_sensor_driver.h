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

//register addresses
#define MODE_CONFIG_REG		  0x09
#define SPO2_CONFIG_REG		  0x0A
#define LED1_PA_REG			  0x0C
#define LED2_PA_REG			  0x0D
#define FIFO_CONFIG_REG 	  0x08
#define FIFO_DATA_REG		  0x07
#define TEMP_CONFIG_REG	      0x21
#define TEMP_INT_REG	      0x1F
#define TEMP_FRAC_REG	      0x20
#define INTERRUPT_STATUS_2	  0x01
#define DIE_TEMP_RDY_EN		  0x03


void MAX30102_init();
void MAX30102_WRITE_REGISTER(uint8_t register_Address, uint8_t data);
void MAX30102_READ_REGISTER(uint8_t register_Address, uint8_t *recv_buff, uint8_t recv_size);
float MAX30102_Read_Temperature();
void MAX30102_Start_Temperature_Measurement();


#endif /* INC_MAX_HEART_SENSOR_DRIVER_H_ */
