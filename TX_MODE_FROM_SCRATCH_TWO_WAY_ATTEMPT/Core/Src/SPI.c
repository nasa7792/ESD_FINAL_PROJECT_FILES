/*
 * SPI.c
 *
 *  Created on: Nov 10, 2024
 *      Author: Asus
 */

#include"SPI.h"
#include"stdio.h"
#define AF05  (0x05)


void SPI_INIT(){
	//enable clock for GPIOA
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;
	//set PA5, PA6 and PA7 to alternate function mode
	GPIOA->MODER|=GPIO_MODER_MODE5_1|GPIO_MODER_MODE6_1|GPIO_MODER_MODE7_1;
	//set which type of alternate function is
	GPIOA->AFR[0]|=(AF05<<20)|(AF05<<24)|(AF05<<28);
	//enable clock access to SPI1
	RCC->APB2ENR|=RCC_APB2ENR_SPI1EN;
	//set software slave managment
	SPI1->CR1|=SPI_CR1_SSM|SPI_CR1_SSI;
	//set SPI in master mode
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_MSTR, 1);
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_BR,  3);
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_CPHA, 0);
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_CPOL, 0);
	//MODIFY_FIELD(SPI1->CR1, SPI_CR1_LSBFIRST, 1);
	SPI1->CR2 |= 0x4;
	SPI1->CR1 |= 0x40; // Enabling SPI SPI periph
	MODIFY_FIELD(SPI1->CR1, SPI_CR1_SPE, 1);
}

//send multiple bytes in case size>1
void SPI_TX_MULTI(uint8_t *data_ptr,int size){
int i=0;
uint8_t temp;
while(i<size){
	while(!(SPI1->SR & (SPI_SR_TXE))){}
	SPI1->DR = data_ptr[i];
	i++;
}
/*Wait until TXE is set*/
while(!(SPI1->SR & (SPI_SR_TXE))){}

/*Wait for BUSY flag to reset*/
while((SPI1->SR & (SPI_SR_BSY))){}

/*Clear OVR flag*/
temp = SPI1->DR;
temp = SPI1->SR;
}


void SPI_READ_MULTI(uint8_t *data_ptr,int size){
	while(size)
	{
		/*Send dummy data*/
		SPI1->DR =0;

		/*Wait for RXNE flag to be set*/
		while(!(SPI1->SR & (SPI_SR_RXNE))){}

		/*Read data from data register*/
		*data_ptr++ = (SPI1->DR);
		size--;
	}
}

