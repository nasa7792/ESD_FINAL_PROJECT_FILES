/* ---------------------------------------------------------------------------------
 * Nalin Saxena
 * ECEN 5613 - Fall 2024 - Prof. McClure
 * University of Colorado Boulder
 * Revised 10/12/24
 * File I2C_driver.c
 *  --------------------------------------------------------------------------------
 * This file contains definitions for i2c config , start , stop condition and basic read write functions
   ---------------------------------------------------------------------------------*/

/* -------------------------------------------------- */
//          INCLUDES & DEFINES
/* -------------------------------------------------- */
#include <I2C_driver.h>
#include "delay.h"

/* -------------------------------------------------- */
//          FUNCTION DEFINITIONS
/* -------------------------------------------------- */

void I2C_init_config()
{
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // send clock for I2C1 and port B

	// declare as alternate func
	MODIFY_FIELD(GPIOB->MODER, GPIO_MODER_MODER8, ESF_GPIO_MODER_ALT_FUNC);
	MODIFY_FIELD(GPIOB->MODER, GPIO_MODER_MODER9, ESF_GPIO_MODER_ALT_FUNC);

	// PB8 scl PB9 sda
	MODIFY_FIELD(GPIOB->AFR[1], GPIO_AFRH_AFSEL8, 4);
	MODIFY_FIELD(GPIOB->AFR[1], GPIO_AFRH_AFSEL9, 4);

	// configure as output open drain
	MODIFY_FIELD(GPIOB->OTYPER, GPIO_OTYPER_OT8, 1);
	MODIFY_FIELD(GPIOB->OTYPER, GPIO_OTYPER_OT9, 1);

	// set speed
	GPIOB->OSPEEDR |= (3 << 16) | (3 << 18);

	// select pull up
	GPIOB->PUPDR |= (1 << 16) | (1 << 18);

	// i2c setup begins
	// first reset i2c
	I2C1->CR1 = I2C_CR1_SWRST;
	I2C1->CR1 &= ~I2C_CR1_SWRST;

	// setup peripheral clock freq
	I2C1->CR2 |= 0x18 << 0; // its 24 mhz hex of 24 ! see ioc file

	// i2c ccrr value see thigh calc its 120 in decimal 1000+4000/(1/pckl which is 24))
	I2C1->CCR |= 0x78 << 0;

	// set trise
	I2C1->TRISE |= 0x19 << 0; // 1000/(1pcll)+1

	// enable i2c peripheral
	I2C1->CR1 |= I2C_CR1_PE;
}

void I2C_START_COMS()
{
	// start bit
	I2C1->CR1 |= I2C_CR1_ACK; // enable ACK
	I2C1->CR1 |= I2C_CR1_START;
	while (I2C1->CR2 & I2C_SR1_SB)
	{
	} // wait for start condition to generate
	delay(1);
}

void I2C_WRITE_DATA(uint8_t data)
{
	while (!(I2C1->SR1 & I2C_SR1_TXE))
		; // wait for TXE bit to set
	I2C1->DR = data;
	while (!(I2C1->SR1 & I2C_SR1_BTF))
		; // wait for byte transmission
}

void I2C_SEND_ADDRESS(uint8_t address)
{

	I2C1->DR = address;
	// delay(4);
	while (!(I2C1->SR1 & (1 << 1)))
		; // wait for address bit to be set
	// This bit is cleared by software reading SR1 register followed reading SR2, or by hardware
	uint8_t temp_clear = I2C1->SR1 | I2C1->SR2;
	;
}

void I2C_STOP_COMS()
{
	I2C1->CR1 |= I2C_CR1_STOP; // stop condition
	delay(1);
}

void I2C_READ(uint8_t address_slave, uint8_t *buffer_recv, uint8_t recv_buff_size)
{
	uint8_t remaining_bytes = recv_buff_size;
	// if only one byte has to read
	// refrence Master receiver https://www.st.com/resource/en/reference_manual/dm00119316-stm32f411xc-e-advanced-arm-based-32bit-mcus-stmicroelectronics.pdf
	if (recv_buff_size == 1)
	{
		I2C1->DR = address_slave;
		while (!(I2C1->SR1 & I2C_SR1_ADDR))
			;										// wait for address bit to be set
		I2C1->CR1 &= ~(1 << 10);					// clear ack
		uint8_t temp_clear = I2C1->SR1 | I2C1->SR2; // ev6
		I2C_STOP_COMS();
		while (!(I2C1->SR1 & I2C_SR1_RXNE))
			;
		buffer_recv[0] = I2C1->DR; // recieve data
		return;
	}
	else
	// if multiple bytes have to be read
	{
		I2C_SEND_ADDRESS(address_slave);
		// keep reading and storing in loop also send acks
		while (remaining_bytes > 2)
		{
			while (!(I2C1->SR1 & I2C_SR1_RXNE))
				;
			buffer_recv[recv_buff_size - remaining_bytes] = I2C1->DR;
			I2C1->CR1 |= I2C_CR1_ACK; // send ack master sends this !!
			remaining_bytes--;
		}

		// second last byte nack condition
		while (!(I2C1->SR1 & I2C_SR1_RXNE))
			;
		buffer_recv[recv_buff_size - remaining_bytes] = I2C1->DR;
		I2C1->CR1 &= ~(1 << 10); // clear ack
		I2C_STOP_COMS();		 // stop is sent at n-1 byte
		remaining_bytes--;
		// last byte is read after stop is sent
		while (!(I2C1->SR1 & I2C_SR1_RXNE))
			;
		buffer_recv[recv_buff_size - remaining_bytes] = I2C1->DR;
	}
}
