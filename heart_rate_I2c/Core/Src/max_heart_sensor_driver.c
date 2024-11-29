/*
 * max_heart_sensor_driver.c
 *
 *  Created on: Nov 28, 2024
 *      Author: Asus
 */



#include "max_heart_sensor_driver.h"

void MAX30102_init(){
	I2C_init_config(); //initalize i2c
}

void MAX30102_WRITE_REGISTER(uint8_t register_Address, uint8_t data){
	I2C_START_COMS();
	uint8_t write_masked_address=0xA0;
	I2C_SEND_ADDRESS(write_masked_address); //same as AE
	I2C_WRITE_DATA(register_Address);
	I2C_WRITE_DATA(data);
	I2C_STOP_COMS();
}


void MAX30102_READ_REGISTER(uint8_t register_Address, uint8_t *recv_buff, uint8_t recv_size){
	I2C_START_COMS(); //start i2c
	uint8_t write_masked_address=(MAX30102_BASE_ADDRESS<<1)|WRITE_MASK;
	uint8_t read_masked_address=(MAX30102_BASE_ADDRESS<<1)|READ_MASK; //should be AF
	I2C_SEND_ADDRESS(write_masked_address); //first send slave address
	I2C_WRITE_DATA(register_Address); //send register address
	I2C_START_COMS(); //repeated start
	I2C_READ(read_masked_address,recv_buff,recv_size); //this should be slave with read
	I2C_STOP_COMS();
}
