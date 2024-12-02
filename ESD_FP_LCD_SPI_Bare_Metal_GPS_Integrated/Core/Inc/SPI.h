/*
 * SPI.h
 *
 *  Created on: Nov 10, 2024
 *      Author: Asus
 */

#ifndef INC_SPI_H_
#define INC_SPI_H_

#include"utilities.h"
#include<stm32f411xe.h>
#include "NRF_DRIVER.h"
#define AF05  (0x05)


void SPI_TX_MULTI(uint8_t *data_ptr,int size);
void SPI_READ_MULTI(uint8_t *data_ptr,int size); //read from nrf?
void SPI_INIT();

#endif /* INC_SPI_H_ */
