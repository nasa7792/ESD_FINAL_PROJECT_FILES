/* ---------------------------------------------------------------------------------
 * Nalin Saxena and Abhirath Koushik
 * ECEN 5613 - Fall 2024 - Prof. McClure
 * University of Colorado Boulder
 * Revised 10/12/24
 *  --------------------------------------------------------------------------------
 * This file contains function definitions of state machine related functions.
 * The FSM_INIT initalizes the state machine with default values and execute_sm
 * executes it in a case driven state machine manner.
 * 
 * Nalin Saxena -FSM_INIT ,execute_sm case STATE_HEART_DATA_ACQUIRE , STATE_TRANSMIT_NRF
 * Abhirath Koushik - STATE_GPS_DATA_ACQUIRE, STATE_STORE_DATA_SD_CARD , STATE_PACKET_FORMATION
 * 
   ---------------------------------------------------------------------------------*/

#include <state_machine.h>
#include "NRF_DRIVER.h"
#include "formated_printf.h"

/* -------------------------------------------------- */
//          EXTERN VARIABLES
/* -------------------------------------------------- */
extern volatile char command_ack[32];

/* -------------------------------------------------- */
//          FUNCTION DEFINITIONS
/* -------------------------------------------------- */

void FSM_INIT(StateMachine *sm)
{ // initalize fsm with default values
	sm->current_state = STATE_HEART_DATA_ACQUIRE;
	sm->valid_heart_rate = 0;
	sm->spo2 = 0;
	sm->heart_rate = 0;
	sm->valid_spo2 = 0;
	strcpy(sm->nrf_payload, DEFAULT_ACK);
	sm->dir1 = 0;
	sm->dir2 = 0;
	sm->gps_lat = 0.0;
	sm->gps_long = 0.0;
	sm->temperature = 0.0;
}

void execute_sm(StateMachine *sm)
{ // state driven state machine
	switch (sm->current_state)
	{
	case STATE_HEART_DATA_ACQUIRE: // acquire heart_rate_spo2
		sm->valid_heart_rate = 0;
		sm->spo2 = 0;
		sm->heart_rate = 0;
		sm->valid_spo2 = 0;
		sm->temperature = 0.0;
		// print a log messaging
		print_info("\n \r State machine Status: Acquire Max30102 data \n \r");
		// check if a command is received to disable max chip
		if (strcmp(command_ack, DISABLE_MAX30102) == 0)
		{
			printf("MAX30102 Is disabled ! \n \r");
			MAX30102_WRITE_REGISTER(MODE_CONFIG_REG, 0x0); // this puts the chip in a shutdown power saving mode
			sm->spo2 = 0.0;
			sm->temperature = 0.0;
			sm->current_state = STATE_GPS_DATA_ACQUIRE;
			break;
		}
		MAX30102_WRITE_REGISTER(MODE_CONFIG_REG, 0x3); // turn the chip on and put in spo2 mode
		bool valid_data = false;
		uint32_t timeout_val = 2; // try for 2 iterations to get heart rate data
		acquire_max_30102_data(timeout_val, &(sm->valid_heart_rate), &(sm->spo2), &(sm->heart_rate), &(sm->valid_spo2), &valid_data);
		sm->current_state = STATE_GPS_DATA_ACQUIRE;
		// if data is not found return as timeout
		if (!valid_data)
		{
			print_error("TIMEOUT ENCOUNTERED! \n \r");
		}

		else
		{
			print_success("\n\rMAX30102 DATA SUCCESS \n \r");
			printf("Spo2 : %ld \n \r", sm->spo2);
		}
		// start temperature measurement
		MAX30102_Start_Temperature_Measurement();
		sm->temperature = MAX30102_Read_Temperature();
		printf("temperature value : %f \n \r", sm->temperature);
		break;

	case STATE_GPS_DATA_ACQUIRE:
		print_info("State machine Status: Acquire GPS Data \n \r");
		// check if command is received to disable gps
		if (strcmp(command_ack, DISABLE_GPS) == 0)
		{
			printf("GPS Is disabled ! \n \r");
			sm->gps_lat = 0.0;
			sm->gps_long = 0.0;
		}
		else
		{
			parse_gps_output(&(sm->gps_lat), &(sm->dir1), &(sm->gps_long), &(sm->dir2));
		}
		sm->current_state = STATE_PACKET_FORMATION;
		break;

	case STATE_PACKET_FORMATION:
		//convert data into packet seperate by '-' to be easily decoded by receiver
		float tempr = 0;
		int spo2 = 0;
		char delimiter = '-';
		print_info("\n\rState machine Status: Packet formatting \n \r");
		tempr = sm->temperature;
		spo2 = sm->spo2;
		int payload_idx = 0;
		sm->nrf_payload[payload_idx++] = 'T';
		if (spo2 == INVALID_SPO2)
		{
			spo2 = 0; //default 0 value
		}
		char temp_holder[10];
		sprintf(temp_holder, "%.2f", tempr);
		int i = 0;
		while (temp_holder[i])
		{
			sm->nrf_payload[payload_idx++] = temp_holder[i++];
		}
		sm->nrf_payload[payload_idx++] = delimiter;
		sm->nrf_payload[payload_idx++] = 'O';
		i++;
		sprintf(temp_holder, "%d", spo2);
		i = 0;
		while (temp_holder[i])
		{
			sm->nrf_payload[payload_idx++] = temp_holder[i++];
		}

		char gps_string[50];
		sm->nrf_payload[payload_idx++] = delimiter;
		sprintf(gps_string, "G%.2f%c,%.2f%c", sm->gps_lat, sm->dir1, sm->gps_long, sm->dir2);
		i = 0;
		while (gps_string[i])
		{
			sm->nrf_payload[payload_idx++] = gps_string[i++];
		}

		sm->nrf_payload[payload_idx] = '\0'; // Null-terminate the payload
		printf("Formatted Payload %s size of payload is %d \n\r", sm->nrf_payload, strlen(sm->nrf_payload));

		sm->current_state = STATE_TRANSMIT_NRF;
		break;

	case STATE_TRANSMIT_NRF:
		print_info("\n\rState machine Status: Transmit PayLoad \n \r");
		if (NRF_TX_DATA(sm->nrf_payload, strlen(sm->nrf_payload)) == 1)
		{
			printf("\n \r transmit success payload size was %d \n \r", strlen(sm->nrf_payload));
		}
		sm->current_state = STATE_STORE_DATA_SD_CARD;
		delay(2000);
		break;
	case 		:
		print_info("\n\rState machine Status: Log Data to SD CARD\n \r");
		process_SD_card(sm->nrf_payload);
		sm->current_state = STATE_HEART_DATA_ACQUIRE;
		break;
	}
}
