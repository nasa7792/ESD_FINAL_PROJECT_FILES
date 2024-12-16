/* ---------------------------------------------------------------------------------
 * Nalin Saxena
 *
 * original work- (functions MAX30102_init, MAX30102_WRITE_REGISTER,MAX30102_READ_REGISTER
 *                 MAX30102_Read_Temperature, MAX30102_Start_Temperature_Measurement,MAX30102_init_TEMPERATURE
 *                 MAX30102_read_fifo)
 *
 * ECEN 5613 - Fall 2024 - Prof. McClure
 * University of Colorado Boulder
 * Revised 10/12/24
 * File name : max_heart_sensor_driver.h
 *  --------------------------------------------------------------------------------
 *
 * This file contains function declarations related to communication with the max30102
 *
 * Attribution - the functions check_new_data,nextSample,getRed,getIR,available have been taken
 *               from the following github https://github.com/sparkfun/MAX30105_Particle_Sensor_Breakout/tree/master/Libraries/Arduino/src
 *               file name MAX30105.cpp. I would like to thank the original authours of the library Natahn Siedle and sparkfun developers.
   ---------------------------------------------------------------------------------*/

#ifndef INC_MAX_HEART_SENSOR_DRIVER_H_
#define INC_MAX_HEART_SENSOR_DRIVER_H_

/* -------------------------------------------------- */
//          INCLUDES & DEFINES
/* -------------------------------------------------- */
#include "I2C_driver.h"
#include <string.h>
#include "stdio.h"
#include "spo2_algorithm.h"
#include "delay.h"
#include <stdio.h>

#define MAX30102_BASE_ADDRESS 0x57
#define READ_MASK 1
#define WRITE_MASK 0

// register addresses map
#define MODE_CONFIG_REG 0x09
#define SPO2_CONFIG_REG 0x0A
#define LED1_PA_REG 0x0C
#define LED2_PA_REG 0x0D
#define FIFO_CONFIG_REG 0x08
#define FIFO_DATA_REG 0x07
#define TEMP_CONFIG_REG 0x21
#define TEMP_INT_REG 0x1F
#define TEMP_FRAC_REG 0x20
#define INTERRUPT_STATUS_2 0x01
#define DIE_TEMP_RDY_EN 0x03
#define FIFO_READ_PTR 0x06
#define FIFO_WR_PTR 0x04
#define FIFO_DATA_REG 0x07
#define SLOT_CONFIG 0x11
#define REG_INTR_ENABLE_1 0x02
#define REG_INTR_ENABLE_2 0x03
#define MAX30102_READ_ADDRESS 0xAF
#define MAX30102_WRITE_ADDRESS 0xAE

/* -------------------------------------------------- */
//          FUNCTION DECLARATIONS
/* -------------------------------------------------- */

/*
 * Function to initalize Max30102 in spo2 mode
 * Parameters:
 * 			    none
 * Returns:
 * 			     none
 */
void MAX30102_init();

/*
 * Function to write to max30102 register
 * Parameters:
 * 			    uint8_t register_Address: address of register
 *              uint8_t data            : data to be written
 * Returns:
 * 			     none
 */
void MAX30102_WRITE_REGISTER(uint8_t register_Address, uint8_t data);

/*
 * Function to read from  max30102 register
 * Parameters:
 * 			    uint8_t register_Address      : address of register
 *              uint8_t *recv_buff            : address of recieving buffer
 *              uint8_t recv_size             : recieve buffer size
 * Returns:
 * 			     none
 */
void MAX30102_READ_REGISTER(uint8_t register_Address, uint8_t *recv_buff, uint8_t recv_size);

/*
 * reads the integer and decimal part of the temperature from 2 registers and converts the value
 * to degree centigrade
 * Parameters:
 * 			    none
 * Returns:
 * 			     none
 */
float MAX30102_Read_Temperature();

void MAX30102_Start_Temperature_Measurement();

/*
 * Configures temperature intterupt to start temperature measurement
 * Parameters:
 * 			    none
 * Returns:
 * 			     none
 */
void MAX30102_init_TEMPERATURE();

/*
 * This function reads raw value from the red led and ir led
 * Parameters:
 * 			    uint32_t *red_led: pointer to red led buffer
 *              uint32_t *ir_led:  pointer to ir led buffer
 * Returns:
 *
 * 			     none
 */
void MAX30102_read_fifo(uint32_t *red_led, uint32_t *ir_led);

/* Note- not original piece of code , acknowledgment cited in file header
 *
 * This function reads raw value from the red led and ir led
 * Parameters:
 * 			    none
 * Returns:
 *
 * 			     number of samples
 */
uint16_t check_new_data();

/* Note- not original piece of code , acknowledgment cited in file header
 *
 * This function advances the tail pointer of circular buffer if new data is available
 * Parameters:
 * 			    none
 * Returns:
 *
 * 			     number of samples
 */
void nextSample(void);

/* Note- not original piece of code , acknowledgment cited in file header
 *
 * This function returns red led buffer value
 * Parameters:
 * 			    none
 * Returns:
 *
 * 			   non  most recent red led valuee
 */
uint32_t getRed(void);

/* Note- not original piece of code , acknowledgment cited in file header
 *
 * This function returns IR led buffer value
 * Parameters:
 * 			    none
 * Returns:
 * 			   most recent ir led value
 */
uint32_t getIR(void);

/* Note- not original piece of code , acknowledgment cited in file header
 *
 * This function returns number of samples based on the head and tail position of the circular buffer
 * Parameters:
 * 			    none
 * Returns:
 * 			  number of samples
 */
uint8_t available(void);

/* Note- not original piece of code , acknowledgment cited in file header
 *
 * This is the master function which initiates data fetching from red and IR leds and ultimately calls
 * the maxim_heart_rate_and_oxygen_saturation to get the value of spo2 and heart rate
 * Parameters:
 * 			    timeout_val :  A Programmable number of iterations post which state machine procecceds to next state
 *              int8_t *heart_rate_status: status returned by  maxim_heart_rate_and_oxygen_saturation function wether a valid value for hr was found
 *              int32_t  *spo2_var : stores the spo2
 *              int32_t  *heart_rate_var : stores the heart rate
 *              int8_t *valid_spo2 :status returned by maxim_heart_rate_and_oxygen_saturation function wether a valid value for spo2 was found
 * Returns:
 * 			  none
 */
void acquire_max_30102_data(uint32_t timeout_val, int8_t *heart_rate_status, int32_t *spo2_var, int32_t *heart_rate_var, int8_t *valid_spo2, bool *status);
#define STORAGE_SIZE 4 // Each long is 4 bytes so limit this to fit on your micro

#endif /* INC_MAX_HEART_SENSOR_DRIVER_H_ */
