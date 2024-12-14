/*
 * NRF_DRIVER.c
 *
 *  Created on: Nov 10, 2024
 *      Author: Nalin Saxena
 */

#include "NRF_DRIVER.h"
#include "delay.h"
#include "SPI.h"
#include "utilities.h"
volatile char command_ack[32];

void print(uint8_t data[], uint32_t len)
{
	int i = 0;
	while (data[i])
	{
		putchar(data[i]);
		i++;
	}
	printf("\n \r");
}

/*
this utility function configures pins for chip select and chip enabel
PA4->CSN
PA5->CE
PA6-> no ack led
PA7-> ack led
*/
void init_CSN_CE_PINS()
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER4, ESF_GPIO_MODER_OUTPUT); // CSN pin
	MODIFY_FIELD(GPIOA->MODER, GPIO_MODER_MODER0, ESF_GPIO_MODER_OUTPUT); // CE pin
	MODIFY_FIELD(GPIOB->MODER, GPIO_MODER_MODER6, ESF_GPIO_MODER_OUTPUT); // no ack led
	MODIFY_FIELD(GPIOB->MODER, GPIO_MODER_MODER7, ESF_GPIO_MODER_OUTPUT); //  ack led
}

void CSN_SELECT_NRF()
{
	GPIOA->BSRR |= GPIO_BSRR_BR_4; // pull low to enable nrf
}

void CSN_UNSELECT_NRF()
{
	GPIOA->BSRR |= GPIO_BSRR_BS_4; // make high to disable
}

void NRF_ENABLE()
{
	GPIOA->BSRR |= GPIO_BSRR_BS_0;
}

void NRF_DISABLE()
{
	GPIOA->BSRR |= GPIO_BSRR_BR_0;
}

void NRF_WRITE_REGISTER(uint8_t REG_ADDRESS, uint8_t data)
{
	uint8_t trans_buff[2];
	trans_buff[0] = REG_ADDRESS | W_REGISTER;
	trans_buff[1] = data;
	CSN_SELECT_NRF();
	SPI_TX_MULTI(trans_buff, 2);
	CSN_UNSELECT_NRF();
}

void NRF_WRITE_REG_MULTI_BYTES(uint8_t REG_ADDRESS, uint8_t *data_trans, int size_trans)
{
	uint8_t trans_buff[2];
	trans_buff[0] = REG_ADDRESS | W_REGISTER;
	CSN_SELECT_NRF();
	SPI_TX_MULTI(trans_buff, 1);
	SPI_TX_MULTI(data_trans, size_trans);
	CSN_UNSELECT_NRF();
}

uint8_t NRF_READ_REGISTER(uint8_t REG_ADDRESS)
{
	uint8_t data_returned = 0;
	CSN_SELECT_NRF();
	SPI_TX_MULTI(&REG_ADDRESS, 1);
	SPI_READ_MULTI(&data_returned, 1);
	CSN_UNSELECT_NRF();
	return data_returned;
}

void NRF_READ_REGISTER_MULTI(uint8_t REG_ADDRESS, uint8_t *RECV_BUFFER, int RECV_BUFFER_SIZE)
{
	CSN_SELECT_NRF();
	SPI_TX_MULTI(&REG_ADDRESS, 1);
	SPI_READ_MULTI(RECV_BUFFER, RECV_BUFFER_SIZE);
	CSN_UNSELECT_NRF();
}

void NRF_PTX_CONFIG(uint8_t *Address, uint8_t channel)
{
	NRF_DISABLE();
	NRF_WRITE_REGISTER(RF_CH, channel);				// select channel
	NRF_WRITE_REG_MULTI_BYTES(TX_ADDR, Address, 5); // set address

	uint8_t current_pipe_status = NRF_READ_REGISTER(EN_RXADDR);
	current_pipe_status = current_pipe_status | (1 << 0); // enable pipe 1
	NRF_WRITE_REGISTER(EN_RXADDR, current_pipe_status);
	NRF_WRITE_REG_MULTI_BYTES(RX_ADDR_P0, Address, 5); // pipe address

	NRF_WRITE_REGISTER(CONFIG, 0x0a); // powwr on device and keep in tx mode
	delay(5);
	// NRF_ENABLE();
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

	else
	{
		NRF_WRITE_REGISTER(CONFIG, 0x08);
		NRF_WRITE_REGISTER(EN_AA, 0x3F);
		NRF_WRITE_REGISTER(EN_RXADDR, 0x03);
		NRF_WRITE_REGISTER(SETUP_AW, 0x03);
		NRF_WRITE_REGISTER(SETUP_RETR, 0x03);
		NRF_WRITE_REGISTER(RF_CH, 0x02);
		NRF_WRITE_REGISTER(RF_SETUP, 0x0E);
		NRF_WRITE_REGISTER(STATUS, 0x00);
		uint8_t rx_addr_p0_def[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
		uint8_t rx_addr_p1_def[5] = {0xC2, 0xC2, 0xC2, 0xC2, 0xC2};
		NRF_WRITE_REG_MULTI_BYTES(RX_ADDR_P1, rx_addr_p1_def, 5);
		uint8_t tx_addr_def[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
		NRF_WRITE_REG_MULTI_BYTES(TX_ADDR, tx_addr_def, 5);
		NRF_WRITE_REGISTER(FIFO_STATUS, 0x11);
		NRF_WRITE_REGISTER(DYNPD, 0);
		NRF_WRITE_REGISTER(FEATURE, 0);
	}
}

void NRF_INIT()
{
	SPI_INIT();			// initalize spi communication
	init_CSN_CE_PINS(); // initalize chip select and chip enable pin
	NRF_DISABLE();		// disable the chip
	nrf24_reset(0);		// reset the register

	NRF_WRITE_REGISTER(CONFIG, 0); // during inital config leaving it as default

	NRF_WRITE_REGISTER(EN_RXADDR, 0); // during inital config leaving it as defaul

	NRF_WRITE_REGISTER(SETUP_AW, 0x03); // 0x03 corresponds to 5 bytes for rx/tx address

	NRF_WRITE_REGISTER(RF_CH, 0); // during inital config leaving it as default

	/*
	below config is related to setting up auto ack along with
	auto ack with payload feature of the NRF. This is crucial for setting up the
	bidirectional communication b/w the 2 boards
	*/

	// for DYNP NRF require EN_AA and Requires EN_DPL and ENAA_P0
	NRF_WRITE_REGISTER(DYNPD, 0x3f);	  // enable dynamic payload with ack
	NRF_WRITE_REGISTER(FEATURE, 0x6);	  // enable dynamic payload length
	NRF_WRITE_REGISTER(EN_AA, 0x3f);	  // enable auto ack
	NRF_WRITE_REGISTER(SETUP_RETR, 0xff); // 15 retries, 500µs delay

	NRF_WRITE_REGISTER(RF_SETUP, 0x0e); // Power= 0db, data rate = 2Mbps
}

void NRD_SEND_CMD(uint8_t cmd)
{
	// Pull the CS Pin LOW to select the device
	CSN_SELECT_NRF();

	SPI_TX_MULTI(&cmd, 1);

	// Pull the CS HIGH to release the device
	CSN_UNSELECT_NRF();
}

uint8_t NRF_TX_DATA(uint8_t *data_ptr, uint8_t sizeofdata)
{
	uint8_t tx_fifo_stat;
	uint8_t status_reg;
	CSN_SELECT_NRF();
	uint8_t cmd = W_TX_PAYLOAD;
	SPI_TX_MULTI(&cmd, 1);
	SPI_TX_MULTI(data_ptr, sizeofdata); // send payload
	CSN_UNSELECT_NRF();
	NRF_ENABLE();
	delay(10);
	NRF_DISABLE();
	tx_fifo_stat = NRF_READ_REGISTER(FIFO_STATUS);
	status_reg = NRF_READ_REGISTER(STATUS);

	NRF_ENABLE();
	delay(10);
	if (is_data_on_pipe(0) == 1)
	{
		GPIOB->BSRR |= GPIO_BSRR_BS_7;
		delay(300);
		GPIOB->BSRR |= GPIO_BSRR_BR_7;
		delay(300);
		print_success("ACK RECIEVED FROM PRX NODE ! \n \r");
		NRF_RECV_DATA(command_ack);
		print(command_ack, 32);
	}

	if (status_reg & (1 << 4))
	{
		print_error("\n\rMax number of retransmission Reached !\n \r");
		GPIOB->BSRR |= GPIO_BSRR_BS_6;
		delay(300);
		GPIOB->BSRR |= GPIO_BSRR_BR_6;
		delay(300);
		status_reg = status_reg | (1 << 4) | (1 << 5);
		NRF_WRITE_REGISTER(STATUS, status_reg);
		status_reg = NRF_READ_REGISTER(STATUS);
		print_info("Clearing MAX retransmission flag ! \n \r");
		cmd = FLUSH_TX;
		NRD_SEND_CMD(cmd);
		NRF_WRITE_REGISTER(FIFO_STATUS, 0x11); // reset fifo
	}

	return 0;
}

uint8_t is_data_on_pipe(uint8_t pipenum)
{
	uint8_t status_reg = NRF_READ_REGISTER(STATUS);
	// if 6 th bit is set and respective data pipe is set
	if ((status_reg & (1 << 6)))
	{
		// clear rx_dr
		NRF_WRITE_REGISTER(STATUS, (1 << 6));
		return 1; // data recieved
	}
	return 0;
}

uint8_t NRF_SEND_PAYLOAD_WIDTH_READ(uint8_t cmd)
{
	uint8_t temp;
	while (!(SPI1->SR & (SPI_SR_TXE)))
	{
	}
	SPI1->DR = cmd;
	temp = SPI1->DR;
	temp = SPI1->SR;
	while (!(SPI1->SR & SPI_SR_RXNE))
	{
	}
	return (uint8_t)(SPI1->DR);
}

void NRF_RECV_DATA(uint8_t *data_ptr_RECV)
{

	uint8_t cmd = R_RX_PL_WID;
	uint8_t payLoad_width = 0;
	CSN_SELECT_NRF();
	cmd = R_RX_PAYLOAD;
	SPI_TX_MULTI(&cmd, 1);
	SPI_READ_MULTI(data_ptr_RECV, 10); // recieve data
	CSN_UNSELECT_NRF();
	delay(10);
	cmd = FLUSH_RX;
	NRD_SEND_CMD(cmd); // flush rx fifo
}
