/* ---------------------------------------------------------------------------------
 * Nalin Saxena
 * ECEN 5613 - Fall 2024 - Prof. McClure
 * University of Colorado Boulder
 * Revised 10/12/24
 * File name : NRF_DRIVER.h
 *  --------------------------------------------------------------------------------
 * This file contains function declarations related to basic SPI communication
   ---------------------------------------------------------------------------------*/

#ifndef INC_NRF_DRIVER_H_
#define INC_NRF_DRIVER_H_

/* -------------------------------------------------- */
//          INCLUDES & DEFINES
/* -------------------------------------------------- */
#include "utilities.h"
#include <stm32f411xe.h>
#include "SPI.h"
#include "formated_printf.h"
#include "stdint.h"

/* Reigster Map */
#define CONFIG 0x00
#define EN_AA 0x01
#define EN_RXADDR 0x02
#define SETUP_AW 0x03
#define SETUP_RETR 0x04
#define RF_CH 0x05
#define RF_SETUP 0x06
#define STATUS 0x07
#define RX_ADDR_P0 0x0A
#define RX_ADDR_P1 0x0B
#define TX_ADDR 0x10
#define FIFO_STATUS 0x17
#define DYNPD 0x1C
#define FEATURE 0x1D

/* instriction hex codes */
#define R_REGISTER 0x00
#define W_REGISTER 0x20
#define REGISTER_MASK 0x1F
#define ACTIVATE 0x50
#define R_RX_PL_WID 0x60
#define R_RX_PAYLOAD 0x61
#define W_TX_PAYLOAD 0xA0
#define W_ACK_PAYLOAD 0xA8
#define FLUSH_TX 0xE1
#define FLUSH_RX 0xE2

/* -------------------------------------------------- */
//          FUNCTION DECLARATIONS
/* -------------------------------------------------- */

/*
 * Function to initalize NRF initial configruation device is not powered on post this config.

 * Parameters:
 * 			    none
 * Returns:
 * 			     none
 */
void NRF_INIT();

/* Function to select NRF
 * pulls csn line low to select the nrf device
 * Parameters:
 * 			    none
 * Returns:
 * 			     none
 */
void CSN_SELECT_NRF();

/*Function to deselect NRF
 * pulls csn line high to deselect nrf
 * Parameters:
 * 			    none
 * Returns:
 * 			     none
 */
void CSN_UNSELECT_NRF();

/*Function to enable NRF
 * CE is made high to enable NRF
 * Parameters:
 * 			    none
 * Returns:
 * 			     none
 */
void NRF_ENABLE();

/*Function to disable NRF
 * CE is made low to disable NRF
 * Parameters:
 * 			    none
 * Returns:
 * 			     none
 */
void NRF_DISABLE();

/*Function write one byte of data to a specific register
 * Parameters:
 * 			    uint8_t REG_ADDRESS : address of register
 *          uint8_t data        : data to be written in the regsiter
 * Returns:
 * 			     none
 */
void NRF_WRITE_REGISTER(uint8_t REG_ADDRESS, uint8_t data);

/*Function write multiple bytes of data to starting from a specific register
 * Parameters:
 * 			    uint8_t REG_ADDRESS : address of register
 *          uint8_t *data       : data buffer pointer
 *          int size_trans      : number of bytes to be written
 * Returns:
 * 			     none
 */
void NRF_WRITE_REG_MULTI_BYTES(uint8_t REG_ADDRESS, uint8_t *data_trans, int size_trans);

/*Function to read one byte of data from a register
 * Parameters:
 * 			    uint8_t REG_ADDRESS : address of register
 * Returns:
 * 			     uint8_t data returned from register
 */
uint8_t NRF_READ_REGISTER(uint8_t REG_ADDRESS);

/*Function to read multiple bytes of data
 * Parameters:
 * 			    uint8_t REG_ADDRESS : address of register
 *          uint8_t *RECV_BUFFER : pointer of reciving buffer
 *          int     RECV_BUFFER_SIZE: recieve buffer size
 * Returns:
 * 			     none
 */
void NRF_READ_REGISTER_MULTI(uint8_t REG_ADDRESS, uint8_t *RECV_BUFFER, int RECV_BUFFER_SIZE);

/*Function to configure nrf as ptx-primary transmitter
 * Parameters:
 * 			    uint8_t *Address : address of TX node
 *          uint8_t channel : which nrf channel is to be selected for communication
 * Returns:
 * 			     none
 */
void NRF_PTX_CONFIG(uint8_t *Address, uint8_t channel);

/*Function to configure nrf as prx-primary reciever
 * Parameters:
 * 			    uint8_t *Address : address of TX node
 *          uint8_t channel : which nrf channel is to be selected for communication
 * Returns:
 * 			     none
 */
void NRF_PRX_CONFIG(uint8_t *Address, uint8_t channel);

/* Send commands to nrf
 * Parameters:
 * 			    uint8_t cmd : command to be sent to nrf
 *
 * Returns:
 * 			     none
 */
void NRD_SEND_CMD(uint8_t cmd);

/* recieve data from nrf prx- only used for recieving
 * Parameters:
 * 			    uint8_t *data_ptr_RECV : buffer to store that guy
 *
 * Returns:
 * 			     none
 */
void NRF_RECV_DATA(uint8_t *data_ptr_RECV);

/* check if new data is on the recieving pipe
 * also used to check for new ack messages after transmission
 * Parameters:
 * 			    uint8_t pipe : pipe number
 *
 * Returns:
 * 			     none
 */
uint8_t is_data_on_pipe(uint8_t pipe);

/* function to transmit data packet with nrf
 * this function also calls is_data_on_pipe to check for ack message from prx node
 * Parameters:
 * 			    uint8_t *data_ptr : buffer which contains data to be transmistted
 *          uint8_t sizeofdata: number of bytes
 *
 * Returns:
 * 			     none
 */
uint8_t NRF_TX_DATA(uint8_t *data_ptr, uint8_t sizeofdata);

#endif /* INC_NRF_DRIVER_H_ */
