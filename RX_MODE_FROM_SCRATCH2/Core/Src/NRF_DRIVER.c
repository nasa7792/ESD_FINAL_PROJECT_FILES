	/*
	 * NRF_DRIVER.c
	 *
	 *  Created on: Nov 10, 2024
	 *      Author: Asus
	 */

	#include"NRF_DRIVER.h"
	#include"delay.h"
	#include"SPI.h"
	#include"utilities.h"
#include<stdio.h>

void init_CSN_CE_PINS(){
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;
	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER4, ESF_GPIO_MODER_OUTPUT); // NSS pin for now4 as outpu
	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER0, ESF_GPIO_MODER_OUTPUT); // NSS pin for now4 as outpu
}


	void CSN_SELECT_NRF(){
		  GPIOA->BSRR |= GPIO_BSRR_BR_4; //pull low to enable nrf
	}

	void CSN_UNSELECT_NRF(){
		  GPIOA->BSRR |= GPIO_BSRR_BS_4; //make high to disable
	}

	void NRF_ENABLE(){
		GPIOA->BSRR |= GPIO_BSRR_BS_0;
	}

	void NRF_DISABLE(){
		GPIOA->BSRR |= GPIO_BSRR_BR_0;
	}



	void NRF_WRITE_REGISTER(uint8_t REG_ADDRESS,uint8_t data){
		uint8_t trans_buff[2];
		trans_buff[0]=REG_ADDRESS|W_REGISTER;
		trans_buff[1]=data;
		CSN_SELECT_NRF();
		SPI_TX_MULTI(trans_buff,2);
		CSN_UNSELECT_NRF();

	}
	void NRF_WRITE_REG_MULTI_BYTES(uint8_t REG_ADDRESS,uint8_t* data_trans,int size_trans){
		uint8_t trans_buff[2];
		trans_buff[0]=REG_ADDRESS|W_REGISTER;
		CSN_SELECT_NRF();
		SPI_TX_MULTI(trans_buff,1);
		SPI_TX_MULTI(data_trans,size_trans);
		CSN_UNSELECT_NRF();
	}

	uint8_t NRF_READ_REGISTER(uint8_t REG_ADDRESS){
		uint8_t data_returned=0;
		CSN_SELECT_NRF();
		SPI_TX_MULTI(&REG_ADDRESS,1);
		SPI_READ_MULTI(&data_returned,1);
		CSN_UNSELECT_NRF();
		return data_returned;
	}

	void NRF_READ_REGISTER_MULTI(uint8_t REG_ADDRESS,uint8_t *RECV_BUFFER,int RECV_BUFFER_SIZE){
		CSN_SELECT_NRF();
		SPI_TX_MULTI(&REG_ADDRESS,1);
		SPI_READ_MULTI(RECV_BUFFER,RECV_BUFFER_SIZE);
		CSN_UNSELECT_NRF();
	}

	void NRF_PTX_CONFIG(uint8_t *Address, uint8_t channel){
		NRF_DISABLE();
		NRF_WRITE_REGISTER(RF_CH,channel); //select channel
		NRF_WRITE_REG_MULTI_BYTES(TX_ADDR,Address,5); //set address
		NRF_WRITE_REGISTER(CONFIG,0x0a); //powwr on device and keep in tx mode
		//NRF_ENABLE();
	}

	void NRF_PRX_CONFIG(uint8_t *Address, uint8_t channel){
		NRF_DISABLE();
		NRF_WRITE_REGISTER(RF_CH,channel); //select channel
		uint8_t current_pipe_status=NRF_READ_REGISTER(EN_RXADDR);
		current_pipe_status=current_pipe_status|(1<<1);//enable pipe 1
		NRF_WRITE_REGISTER(EN_RXADDR,current_pipe_status);
		NRF_WRITE_REG_MULTI_BYTES(RX_ADDR_P1,Address,5); //pipe address
		NRF_WRITE_REGISTER(RX_PW_P1,32); //data 32 bytes width
		NRF_WRITE_REGISTER(CONFIG,0x0b); //powwr on device and keep in tx mode
		delay(5);
		//NRF_ENABLE();
	}

	void nrf24_reset(uint8_t REG)
	{
		if (REG == STATUS)
		{
			NRF_WRITE_REGISTER(STATUS, 0x00);
		}

		else if (REG == FIFO_STATUS)
		{
			NRF_WRITE_REGISTER(FIFO_STATUS, 0x11);
		}

		else {
		NRF_WRITE_REGISTER(CONFIG, 0x08);
		NRF_WRITE_REGISTER(EN_AA, 0x3F);
		NRF_WRITE_REGISTER(EN_RXADDR, 0x03);
		NRF_WRITE_REGISTER(SETUP_AW, 0x03);
		NRF_WRITE_REGISTER(SETUP_RETR, 0x03);
		NRF_WRITE_REGISTER(RF_CH, 0x02);
		NRF_WRITE_REGISTER(RF_SETUP, 0x0E);
		NRF_WRITE_REGISTER(STATUS, 0x00);
		NRF_WRITE_REGISTER(OBSERVE_TX, 0x00);
		NRF_WRITE_REGISTER(CD, 0x00);
		uint8_t rx_addr_p0_def[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
		NRF_WRITE_REG_MULTI_BYTES(RX_ADDR_P0, rx_addr_p0_def, 5);
		uint8_t rx_addr_p1_def[5] = {0xC2, 0xC2, 0xC2, 0xC2, 0xC2};
		NRF_WRITE_REG_MULTI_BYTES(RX_ADDR_P1, rx_addr_p1_def, 5);
		NRF_WRITE_REGISTER(RX_ADDR_P2, 0xC3);
		NRF_WRITE_REGISTER(RX_ADDR_P3, 0xC4);
		NRF_WRITE_REGISTER(RX_ADDR_P4, 0xC5);
		NRF_WRITE_REGISTER(RX_ADDR_P5, 0xC6);
		uint8_t tx_addr_def[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
		NRF_WRITE_REG_MULTI_BYTES(TX_ADDR, tx_addr_def, 5);
		NRF_WRITE_REGISTER(RX_PW_P0, 0);
		NRF_WRITE_REGISTER(RX_PW_P1, 0);
		NRF_WRITE_REGISTER(RX_PW_P2, 0);
		NRF_WRITE_REGISTER(RX_PW_P3, 0);
		NRF_WRITE_REGISTER(RX_PW_P4, 0);
		NRF_WRITE_REGISTER(RX_PW_P5, 0);
		NRF_WRITE_REGISTER(FIFO_STATUS, 0x11);
		NRF_WRITE_REGISTER(DYNPD, 0);
		NRF_WRITE_REGISTER(FEATURE, 0);
		}
	}


	void NRF_INIT(){
		SPI_INIT(); //initalize spi comms
		init_CSN_CE_PINS();
		NRF_DISABLE();
		nrf24_reset(0);
		NRF_WRITE_REGISTER(CONFIG, 0);  // will be configured later

		NRF_WRITE_REGISTER(EN_AA, 0x3f);  // No Auto ACK
		//NRF_WRITE_REGISTER(SETUP_RETR, 0x03); // 15 retries, 500µs delay

		NRF_WRITE_REGISTER (EN_RXADDR, 0);  // Not Enabling any data pipe right now

		NRF_WRITE_REGISTER (SETUP_AW, 0x03);  // 5 Bytes for the TX/RX address

		NRF_WRITE_REGISTER (SETUP_RETR, 0x3);   // No retransmission

		NRF_WRITE_REGISTER (RF_CH, 0);  // will be setup during Tx or RX

		NRF_WRITE_REGISTER (RF_SETUP, 0x0e);   // Power= 0db, data rate = 2Mbps
		//NRF_ENABLE();
	}


	void NRD_SEND_CMD (uint8_t cmd)
	{
		// Pull the CS Pin LOW to select the device
		CSN_SELECT_NRF();

		SPI_TX_MULTI(&cmd, 1);

		// Pull the CS HIGH to release the device
		CSN_UNSELECT_NRF();
	}


	uint8_t NRF_TX_DATA(uint8_t *data_ptr){
		uint8_t tx_fifo_stat;
		CSN_SELECT_NRF();
	  uint8_t cmd = W_TX_PAYLOAD;
	  SPI_TX_MULTI( &cmd, 1);
		SPI_TX_MULTI(data_ptr,32);//send payload
		CSN_UNSELECT_NRF();
//		delay(10);

		NRF_ENABLE();
		delay(10);
		NRF_DISABLE();
		tx_fifo_stat=NRF_READ_REGISTER(FIFO_STATUS);
	//	printf("tx_fifo_stat is - %d",tx_fifo_stat);
//		if(tx_fifo_stat&(1<<5)){
//			printf("ack recieved !\n \r");
//			 NRF_WRITE_REGISTER(STATUS, (1 << 5));
//		     cmd = FLUSH_TX;
//		     NRD_SEND_CMD(cmd);
//		     return 1;
//		}
		if ((tx_fifo_stat&(1<<4)) && (!(tx_fifo_stat&(1<<3)))){
			cmd=FLUSH_TX;
			NRD_SEND_CMD(cmd);
			NRF_WRITE_REGISTER(FIFO_STATUS, 0x11); //reset fifo
			return 1;
		}
		return 0;

	}

uint8_t is_data_on_pipe(uint8_t pipenum){
	NRF_ENABLE();
uint8_t status_reg=NRF_READ_REGISTER(STATUS);
//if 6 th bit is set and respective data pipe is set
if((status_reg & (1<<6))&& status_reg &(1<<pipenum)){
//clear rx_dr
	NRF_WRITE_REGISTER(STATUS,(1<<6));
	return 1; //data recieved
}
return 0;
}

void NRF_RECV_DATA(uint8_t *data_ptr_RECV){
	uint8_t cmd = R_RX_PAYLOAD;
	CSN_SELECT_NRF();
	SPI_TX_MULTI( &cmd, 1);
	SPI_READ_MULTI(data_ptr_RECV,32);//recieve data
	CSN_UNSELECT_NRF();
	delay(10);
	cmd=FLUSH_RX;
	NRD_SEND_CMD(cmd); //flush rx fifo
	NRF_DISABLE();
}


