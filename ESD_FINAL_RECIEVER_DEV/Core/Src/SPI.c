/* ---------------------------------------------------------------------------------
 * Nalin Saxena
 * ECEN 5613 - Fall 2024 - Prof. McClure
 * University of Colorado Boulder
 * Revised 10/12/24
 * File SPI.c
 *  --------------------------------------------------------------------------------
 * This file contains definitions for spi config and basic read write operations.
   ---------------------------------------------------------------------------------*/

/* -------------------------------------------------- */
//          INCLUDES & DEFINES
/* -------------------------------------------------- */
#include "SPI.h"
#include "stdio.h"

/* -------------------------------------------------- */
//          FUNCTION DEFINITIONS
/* -------------------------------------------------- */

void SPI_INIT()
{
	// enable clock for gpioA peripheral
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	// declare pa5,pa6,pa7 as alternate functional pins
	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER5, ESF_GPIO_MODER_ALT_FUNC); // PA5-> SPI1_SCK
	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER6, ESF_GPIO_MODER_ALT_FUNC); // PA6-> SPI1_MISO
	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER7, ESF_GPIO_MODER_ALT_FUNC); // PA7-> SPI1_MOSI

	MODIFY_FIELD(GPIOA->AFR[0], GPIO_AFRL_AFSEL5, 5);
	MODIFY_FIELD(GPIOA->AFR[0], GPIO_AFRL_AFSEL6, 5);
	MODIFY_FIELD(GPIOA->AFR[0], GPIO_AFRL_AFSEL7, 5);

	// enable clock for spi1 peripheral
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	// enable software slave management
	SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;

	// configure spi in master mode
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_MSTR, 1);
	// set spi baud rate
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_BR, 3);
	// set cpol and cpha as 0,0 spi mode 0
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_CPHA, 0);
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_CPOL, 0);

	SPI1->CR2 |= 0x4;
	SPI1->CR1 |= 0x40;
	// Enabling SPI SPI peripheral
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_SPE, 1);
}

// send multiple bytes in case size>1
void SPI_TX_MULTI(uint8_t *data_ptr, int size)
{
	int i = 0;

	while (i < size)
	{
		while (!(SPI1->SR & (SPI_SR_TXE)))
		{
		}
		SPI1->DR = data_ptr[i];
		i++;
	}
	/*Wait until TXE is set*/
	while (!(SPI1->SR & (SPI_SR_TXE)))
	{
	}

	/*Wait for BUSY flag to reset*/
	while ((SPI1->SR & (SPI_SR_BSY)))
	{
	}

	uint8_t software_clear_OVR_FLAG;
	/*read DR first and then SR to clear OVR flag to avoid interruptions in spi*/
	software_clear_OVR_FLAG = SPI1->DR;
	software_clear_OVR_FLAG = SPI1->SR;
}

void SPI_READ_MULTI(uint8_t *data_ptr, int size)
{
	while (size)
	{
		/*Send dummy data*/
		SPI1->DR = 0;

		/*Wait for RXNE flag to be set*/
		while (!(SPI1->SR & (SPI_SR_RXNE)))
		{
		}

		/*Read data from data register*/
		*data_ptr++ = (SPI1->DR);
		size--;
	}
}