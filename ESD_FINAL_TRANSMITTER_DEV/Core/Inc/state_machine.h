/* ---------------------------------------------------------------------------------
 * Nalin Saxena
 * ECEN 5613 - Fall 2024 - Prof. McClure
 * University of Colorado Boulder
 * Revised 10/12/24
 *  --------------------------------------------------------------------------------
 * This file contains the software architecture of the state machine which the transmitter
 * uses to transition b/w various states of data acquistion from different sensors and transmitting 
 * via the radio frequencu module.
   ---------------------------------------------------------------------------------*/
#ifndef INC_STATE_MACHINE_H_
#define INC_STATE_MACHINE_H_

/* -------------------------------------------------- */
//          INCLUDES & DEFINES
/* -------------------------------------------------- */
#include<stdint.h>
#include<stdbool.h>
#include<string.h>
#include"max_heart_sensor_driver.h"
#include"gps.h"
#include"sd_card.h"

// an enum to define various state transitions
typedef enum {
STATE_HEART_DATA_ACQUIRE,
STATE_GPS_DATA_ACQUIRE,
STATE_PACKET_FORMATION,
STATE_STORE_DATA_SD_CARD,
STATE_TRANSMIT_NRF,
} current_state;

//state machine variables to store data from different modules
typedef struct {
	current_state current_state; 
    int8_t valid_heart_rate; 
    int32_t spo2; 
    int32_t heart_rate;
    int8_t valid_spo2; 
    float gps_lat;
	float gps_long;
	char dir1;
	char dir2;
	float temperature;
    uint8_t nrf_payload[32];
} StateMachine;

/* -------------------------------------------------- */
//          FUNCTION DECLARATIONS
/* -------------------------------------------------- */

/*
 * Function to initialize the state machine with default values and
 * define initial entry state.
 * 
 * Parameters:
 * 			 StateMachine *sm : pointer to the state machine instance
 *     
 * Returns:
 * 			none
 */
void FSM_INIT(StateMachine *sm);

/*
 * Function to execute the main state machine 
 * 
 * Parameters:
 * 			 StateMachine *sm : pointer to the state machine instance
 *     
 * Returns:
 * 			none
 */
void execute_sm(StateMachine *sm);
#endif /* INC_STATE_MACHINE_H_ */
