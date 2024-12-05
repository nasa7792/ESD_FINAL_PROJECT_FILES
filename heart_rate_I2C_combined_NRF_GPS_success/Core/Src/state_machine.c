/*
 * state_machine.c
 *
 *  Created on: Dec 3, 2024
 *      Author: Asus
 */


#include<state_machine.h>
#include"NRF_DRIVER.h"



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
			bool valid_data=false;
			uint32_t timeout_val=2; //try for 10 seconds to get heart rate data
			acquire_max_30102_data(timeout_val,&(sm->valid_heart_rate),&(sm->spo2),&(sm->heart_rate),&(sm->valid_spo2),&valid_data);
			sm->current_state=STATE_GPS_DATA_ACQUIRE;
			if(!valid_data){
				printf("timeout! \n \r");
			}
			break;

		case STATE_GPS_DATA_ACQUIRE:
			parse_gps_output(&(sm->gps_lat), &(sm->dir1), &(sm->gps_long), &(sm->dir2));
			sm->current_state=STATE_PACKET_FORMATION;
			break;

		case STATE_PACKET_FORMATION:
			int hr=0;
			int spo2=0;
			printf("state changed to new state sending dummy packet \n \r");
			hr=sm->heart_rate;
			spo2=sm->spo2;
			int payload_idx=0;
			sm->nrf_payload[payload_idx++]='H';
			if(hr==-999 ||spo2==-999){
				printf("Invalid \n \r");
				hr=0;spo2=0;
			}
			printf("hr %d \n \r",hr);
			printf("spo2 %d \n \r",spo2);
			char temp_holder[10];
			sprintf(temp_holder, "%d", hr);
			printf("%s \n \r",temp_holder);
			int i=0;
			while(temp_holder[i]){
			sm->nrf_payload[payload_idx++]=temp_holder[i++];
			}
			sm->nrf_payload[payload_idx++]='O';
			i++;
			sprintf(temp_holder, "%d", spo2);
			i=0;
			while(temp_holder[i]){
			sm->nrf_payload[payload_idx++]=temp_holder[i++];
			}

			char gps_string[50];
			sprintf(gps_string, "G%.2f%c,%.2f%c", sm->gps_lat, sm->dir1, sm->gps_long, sm->dir2);
			i = 0;
			while (gps_string[i]) {
				sm->nrf_payload[payload_idx++] = gps_string[i++];
			}

			sm->nrf_payload[payload_idx] = '\0'; // Null-terminate the payload
			printf("transmitted string %s\n", sm->nrf_payload);

			sm->current_state = STATE_TRANSMIT_NRF;
			break;

		case STATE_TRANSMIT_NRF:
			if(NRF_TX_DATA(sm->nrf_payload,strlen(sm->nrf_payload))==1){
			printf("\n \r transmit success payload size was %d \n \r",strlen(sm->nrf_payload));
			}
			else{
				printf("\n \r Something went wrong ! \n \r");
			}
			sm->current_state=STATE_STORE_DATA_SD_CARD;
			break;
		case STATE_STORE_DATA_SD_CARD:
			process_SD_card(sm->nrf_payload);
			sm->current_state=STATE_HEART_DATA_ACQUIRE;
			break;
	}

}
