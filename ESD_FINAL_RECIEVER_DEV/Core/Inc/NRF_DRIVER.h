/*
 * NRF_DRIVER.h
 *
 *  Created on: Nov 10, 2024
 *      Author: Asus
 */

#ifndef INC_NRF_DRIVER_H_
#define INC_NRF_DRIVER_H_


#include"utilities.h"
#include<stm32f411xe.h>
#include"SPI.h"


#include "stdint.h"


/* Memory Map */
#define CONFIG      0x00
#define EN_AA       0x01
#define EN_RXADDR   0x02
#define SETUP_AW    0x03
#define SETUP_RETR  0x04
#define RF_CH       0x05
#define RF_SETUP    0x06
#define STATUS      0x07
#define OBSERVE_TX  0x08
#define CD          0x09
#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F
#define TX_ADDR     0x10
#define RX_PW_P0    0x11
#define RX_PW_P1    0x12
#define RX_PW_P2    0x13
#define RX_PW_P3    0x14
#define RX_PW_P4    0x15
#define RX_PW_P5    0x16
#define FIFO_STATUS 0x17
#define DYNPD	    0x1C
#define FEATURE	    0x1D

/* Instruction Mnemonics */
#define R_REGISTER    0x00
#define W_REGISTER    0x20
#define REGISTER_MASK 0x1F
#define ACTIVATE      0x50
#define R_RX_PL_WID   0x60
#define R_RX_PAYLOAD  0x61
#define W_TX_PAYLOAD  0xA0
#define W_ACK_PAYLOAD 0xA8
#define FLUSH_TX      0xE1
#define FLUSH_RX      0xE2
#define REUSE_TX_PL   0xE3
#define NOP           0xFF

void NRF_INIT();
void CSN_SELECT_NRF();
void CSN_UNSELECT_NRF();
void NRF_ENABLE();
void NRF_DISABLE();
void NRF_WRITE_REGISTER(uint8_t REG_ADDRESS,uint8_t data);
void NRF_WRITE_REG_MULTI_BYTES(uint8_t REG_ADDRESS,uint8_t* data_trans,int size_trans);
uint8_t NRF_READ_REGISTER(uint8_t REG_ADDRESS);
void NRF_READ_REGISTER_MULTI(uint8_t REG_ADDRESS,uint8_t *RECV_BUFFER,int RECV_BUFFER_SIZE);
void init_CSN_CE_PINS();
void NRF_PTX_CONFIG(uint8_t *Address, uint8_t channel);
void NRF_PRX_CONFIG(uint8_t *Address, uint8_t channel);
void NRD_SEND_CMD(uint8_t cmd);
uint8_t NRF_TX_DATA(uint8_t *data_ptr);
uint8_t NRF_RECV_DATA(uint8_t *data_ptr_RECV);
uint8_t is_data_on_pipe(uint8_t pipe);
uint8_t NRF_SEND_PAYLOAD_WIDTH_READ(uint8_t cmd);



#endif /* INC_NRF_DRIVER_H_ */
