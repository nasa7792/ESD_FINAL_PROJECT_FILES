/*
 * state_machine.c
 *
 *  Created on: Dec 3, 2024
 *      Author: Asus
 */


#include<state_machine.h>
#include"NRF_DRIVER.h"
#include"formated_printf.h"



void FSM_INIT(StateMachine *sm){
	sm->current_state=STATE_HEART_DATA_ACQUIRE;
	sm->valid_heart_rate=0;
	sm->spo2=0;
	sm->heart_rate=0;
	sm->valid_spo2=0;
	strcpy(sm->nrf_payload,"default_payload");
	sm->dir1=0;
	sm->dir2=0;
	sm->gps_lat=0.0;
	sm->gps_long=0.0;
}

void execute_sm(StateMachine *sm){
	switch(sm->current_state){
		case STATE_HEART_DATA_ACQUIRE:
			sm->valid_heart_rate=0;
			sm->spo2=0;
			sm->heart_rate=0;
			sm->valid_spo2=0;
			print_info("\n \r State machine Status: Acquire Max30102 data \n \r");
			bool valid_data=false;
			uint32_t timeout_val=2; //try for 10 seconds to get heart rate data
			acquire_max_30102_data(timeout_val,&(sm->valid_heart_rate),&(sm->spo2),&(sm->heart_rate),&(sm->valid_spo2),&valid_data);
			sm->current_state=STATE_GPS_DATA_ACQUIRE;
			if(!valid_data){
				print_error("TIMEOUT ENCOUNTERED! \n \r");
			}
			else{
				 print_success("\n\rMAX30102 DATA SUCCESS \n \r");
				 printf("Heart rate: %ld \n \r",sm->heart_rate);
				 printf("Spo2 : %ld \n \r",sm->spo2);
			}
			break;

		case STATE_GPS_DATA_ACQUIRE:
			print_info("State machine Status: Acquire GPS Data \n \r");
			parse_gps_output(&(sm->gps_lat), &(sm->dir1), &(sm->gps_long), &(sm->dir2));
			sm->current_state=STATE_PACKET_FORMATION;
			break;

		case STATE_PACKET_FORMATION:
			int hr=0;
			int spo2=0;
			char delimiter='-';
			print_info("\n\rState machine Status: Packet formatting \n \r");
			hr=sm->heart_rate;
			spo2=sm->spo2;
			int payload_idx=0;
			sm->nrf_payload[payload_idx++]='H';
			if(hr==-999 ||spo2==-999){
				hr=0;spo2=0;
			}
			char temp_holder[10];
			sprintf(temp_holder, "%d", hr);
			int i=0;
			while(temp_holder[i]){
			sm->nrf_payload[payload_idx++]=temp_holder[i++];
			}
			sm->nrf_payload[payload_idx++]=delimiter;
			sm->nrf_payload[payload_idx++]='O';
			i++;
			sprintf(temp_holder, "%d", spo2);
			i=0;
			while(temp_holder[i]){
			sm->nrf_payload[payload_idx++]=temp_holder[i++];
			}

			char gps_string[50];
			sm->nrf_payload[payload_idx++]=delimiter;
			sprintf(gps_string, "G%.2f%c,%.2f%c", sm->gps_lat, sm->dir1, sm->gps_long, sm->dir2);
			i = 0;
			while (gps_string[i]) {
				sm->nrf_payload[payload_idx++] = gps_string[i++];
			}

			sm->nrf_payload[payload_idx] = '\0'; // Null-terminate the payload
			printf("Formatted Payload %s size of payload is %d \n\r", sm->nrf_payload,strlen(sm->nrf_payload));

			sm->current_state = STATE_TRANSMIT_NRF;
			break;

		case STATE_TRANSMIT_NRF:
			print_info("\n\rState machine Status: Transmit PayLoad \n \r");
			if(NRF_TX_DATA(sm->nrf_payload,strlen(sm->nrf_payload))==1){
			printf("\n \r transmit success payload size was %d \n \r",strlen(sm->nrf_payload));
			}
			sm->current_state=STATE_STORE_DATA_SD_CARD;
			delay(2000);
			break;
		case STATE_STORE_DATA_SD_CARD:
			print_info("\n\rState machine Status: Log Data to SD CARD\n \r");
			process_SD_card(sm->nrf_payload);
			sm->current_state=STATE_HEART_DATA_ACQUIRE;
			break;
	}

}
