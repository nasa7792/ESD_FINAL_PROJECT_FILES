/*
 * max_heart_sensor_driver.c
 *
 *  Created on: Nov 28, 2024
 *      Author: Asus
 */



#include "max_heart_sensor_driver.h"

void MAX30102_init(){
	I2C_init_config(); //initalize i2c
	   // Reset the sensor
	    MAX30102_WRITE_REGISTER(MODE_CONFIG_REG, 0x40); // Reset the device

	    // Configure for SpO₂ mode
	    MAX30102_WRITE_REGISTER(MODE_CONFIG_REG, 0x03); // Heart rate + SpO₂ mode

	    // LED Pulse Amplitude
	    MAX30102_WRITE_REGISTER(LED1_PA_REG, 0x24); // Set RED LED pulse amplitude
	    MAX30102_WRITE_REGISTER(LED2_PA_REG, 0x24); // Set IR LED pulse amplitude

	    // SpO₂ Configuration
	    MAX30102_WRITE_REGISTER(SPO2_CONFIG_REG, 0x27);
	    // SPO2_ADC Range: 4096 nA (bit 5-6 = 01)
	    // SPO2 Sample Rate: 100 Hz (bit 2-4 = 100)
	    // LED Pulse Width: 411 µs (bit 0-1 = 11)

	    // FIFO Configuration
	    MAX30102_WRITE_REGISTER(FIFO_CONFIG_REG, 0x02);
	    // FIFO_Average: No averaging, direct samples
}



void MAX30102_WRITE_REGISTER(uint8_t register_Address, uint8_t data){
	I2C_START_COMS();
	uint8_t write_masked_address=0xAE;
	I2C_SEND_ADDRESS(write_masked_address); //same as AE
	I2C_WRITE_DATA(register_Address);
	I2C_WRITE_DATA(data);
	I2C_STOP_COMS();
}


void MAX30102_READ_REGISTER(uint8_t register_Address, uint8_t *recv_buff, uint8_t recv_size){

	uint8_t write_masked_address=0xAE;
	uint8_t read_masked_address=0xAF; //should be AF
	I2C_START_COMS(); //start i2c
	I2C_SEND_ADDRESS(write_masked_address); //first send slave address
	I2C_WRITE_DATA(register_Address); //send register address
	I2C_START_COMS(); //repeated start
	I2C_READ(read_masked_address,recv_buff,recv_size); //this should be slave with read
//	I2C_STOP_COMS(); this caused 2 hours of debugging
}

void MAX30102_Start_Temperature_Measurement() {
    MAX30102_WRITE_REGISTER(TEMP_CONFIG_REG, 0x01); // Start temperature conversion
    MAX30102_WRITE_REGISTER(DIE_TEMP_RDY_EN, 0x02); // Start temperature conversion
}


float MAX30102_Read_Temperature() {
    uint8_t temp_int, temp_frac;

    // Read the temperature integer part
    MAX30102_READ_REGISTER(TEMP_INT_REG, &temp_int, 1);

    // Read the temperature fractional part
    MAX30102_READ_REGISTER(TEMP_FRAC_REG, &temp_frac, 1);

    // Combine the integer and fractional parts
    float temperature = temp_int + (temp_frac * 0.0625);
    //printf("hi one read was done but then something happened %d \n \r",temp_frac);
    return temperature;
}
