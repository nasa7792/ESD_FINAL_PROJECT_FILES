/*
 * state_machine.h
 *
 *  Created on: Dec 3, 2024
 *      Author: Asus
 */

#ifndef INC_STATE_MACHINE_H_
#define INC_STATE_MACHINE_H_

#include<stdint.h>
#include<stdbool.h>
#include<string.h>
#include"max_heart_sensor_driver.h"
#include"gps.h"
#include"sd_card.h"

typedef enum {
STATE_HEART_DATA_ACQUIRE,
STATE_GPS_DATA_ACQUIRE,
STATE_PACKET_FORMATION,
STATE_STORE_DATA_SD_CARD,
STATE_TRANSMIT_NRF,
STATE_EXECUTE_RECV_COMMAND

} current_state;

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
    uint8_t nrf_payload[32];
} StateMachine;

void FSM_INIT(StateMachine *sm);
void execute_sm(StateMachine *sm);
#endif /* INC_STATE_MACHINE_H_ */
