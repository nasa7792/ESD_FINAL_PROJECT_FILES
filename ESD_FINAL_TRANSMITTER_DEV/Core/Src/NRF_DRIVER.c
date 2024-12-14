/* ---------------------------------------------------------------------------------
 * Nalin Saxena
 * ECEN 5613 - Fall 2024 - Prof. McClure
 * University of Colorado Boulder
 * Revised 10/12/24
 *  --------------------------------------------------------------------------------
 * This file is the nrf driver implementation. It has key functions to configure an nrf node
 * as PTX or a PRX and also the data transmission with ack.
   ---------------------------------------------------------------------------------*/

/* -------------------------------------------------- */
//          INCLUDES & DEFINES
/* -------------------------------------------------- */
#include "NRF_DRIVER.h"
#include "delay.h"
#include "SPI.h"
#include "utilities.h"
/* -------------------------------------------------- */
//          GLOBALS
/* -------------------------------------------------- */
volatile char command_ack[32];


/* -------------------------------------------------- */
//          FUNCTION DEFINITIONS
/* -------------------------------------------------- */

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
	GPIOA->BSRR |= GPIO_BSRR_BS_0; // enable nrf
}

void NRF_DISABLE()
{
	GPIOA->BSRR |= GPIO_BSRR_BR_0; // disable nrf
}

void NRF_WRITE_REGISTER(uint8_t REG_ADDRESS, uint8_t data)
{
	uint8_t trans_buff[2];					  // create a temporary transmit buffer
	trans_buff[0] = REG_ADDRESS | W_REGISTER; // send register address with Write command to nrf
	trans_buff[1] = data;
	CSN_SELECT_NRF();
	SPI_TX_MULTI(trans_buff, 2); // transmit via spi
	CSN_UNSELECT_NRF();
}

void NRF_WRITE_REG_MULTI_BYTES(uint8_t REG_ADDRESS, uint8_t *data_trans, int size_trans)
{ // this function is mainlu used to write the address of the ptx or prx node
	uint8_t trans_buff[2];
	trans_buff[0] = REG_ADDRESS | W_REGISTER;
	CSN_SELECT_NRF();
	SPI_TX_MULTI(trans_buff, 1);		  // send register address with Write command to nrf
	SPI_TX_MULTI(data_trans, size_trans); // transmit the data_trans buffer in one shot
	CSN_UNSELECT_NRF();
}

uint8_t NRF_READ_REGISTER(uint8_t REG_ADDRESS)
{
	uint8_t data_returned = 0; // a variable to store data read from register
	CSN_SELECT_NRF();
	SPI_TX_MULTI(&REG_ADDRESS, 1); // send register address
	SPI_READ_MULTI(&data_returned, 1);
	CSN_UNSELECT_NRF();
	return data_returned; // return data
}

void NRF_READ_REGISTER_MULTI(uint8_t REG_ADDRESS, uint8_t *RECV_BUFFER, int RECV_BUFFER_SIZE)
{
	CSN_SELECT_NRF();
	SPI_TX_MULTI(&REG_ADDRESS, 1);				   // send register address
	SPI_READ_MULTI(RECV_BUFFER, RECV_BUFFER_SIZE); // read RECV_BUFFER_SIZE number of bytes and store in  RECV_BUFFER
	CSN_UNSELECT_NRF();
}

void NRF_PTX_CONFIG(uint8_t *Address, uint8_t channel)
{
	NRF_DISABLE();									// first disable nrf
	NRF_WRITE_REGISTER(RF_CH, channel);				// select channel
	NRF_WRITE_REG_MULTI_BYTES(TX_ADDR, Address, 5); // set address

	uint8_t current_pipe_status = NRF_READ_REGISTER(EN_RXADDR);
	current_pipe_status = current_pipe_status | (1 << 0); // enable pipe 0
	NRF_WRITE_REGISTER(EN_RXADDR, current_pipe_status);	  // enable pipe 0 as receiving pipe as well
	NRF_WRITE_REG_MULTI_BYTES(RX_ADDR_P0, Address, 5);	  // pipe address

	NRF_WRITE_REGISTER(CONFIG, 0x0a); // power on device and keep in tx mode
	delay(5);
	// NRF_ENABLE(); //nrf should be kept disabled
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
	uint8_t status_reg; // a variable to store  status_reg info
	/*
	the data sheet specifies that first data has to be written with TX command and then
	a pulse on the CE pin for atleast 10uS actually transmits the data
	*/
	CSN_SELECT_NRF();
	uint8_t cmd = W_TX_PAYLOAD; // send write payload command
	SPI_TX_MULTI(&cmd, 1);
	SPI_TX_MULTI(data_ptr, sizeofdata); // send payload
	CSN_UNSELECT_NRF();
	// pulse ce
	NRF_ENABLE();
	delay(10);
	NRF_DISABLE();
	tx_fifo_stat = NRF_READ_REGISTER(FIFO_STATUS);
	// read status register
	status_reg = NRF_READ_REGISTER(STATUS);
	/*post transmission the ptx is converted to receiver and checks for an ack message*/
	NRF_ENABLE(); // enable nrf to conver it into receiver mode
	delay(10);
	// chceck for data on pipe 0
	if (is_data_on_pipe(0) == 1)
	{
		// if data is received on pipe 0 means an ack message is recieved
		// we blink ack (green) led
		GPIOB->BSRR |= GPIO_BSRR_BS_7;
		delay(300);
		GPIOB->BSRR |= GPIO_BSRR_BR_7;
		delay(300);
		print_success("ACK RECIEVED FROM PRX NODE ! \n \r");
		NRF_RECV_DATA(command_ack); // call receive function to read payload
		print(command_ack, 32);		// print the payload
	}
	/*error handling*/
	if (status_reg & (MAX_NUMBER_OF_RETRANSMISSION_MASK))
	{
		// if max number of retranmission is rached and no ack is received the bit has to be cleared to enable
		// further communication
		print_error("\n\rMax number of retransmission Reached !\n \r");
		// blink no-ack (red led)
		GPIOB->BSRR |= GPIO_BSRR_BS_6;
		delay(300);
		GPIOB->BSRR |= GPIO_BSRR_BR_6;
		delay(300);
		// clear MAX_NUMBER_OF_RETRANSMISSION_MASK bit by writing a one
		status_reg = status_reg | (1 << 4) | (1 << 5);
		NRF_WRITE_REGISTER(STATUS, status_reg);
		status_reg = NRF_READ_REGISTER(STATUS);
		print_info("Clearing MAX retransmission flag ! \n \r");
		cmd = FLUSH_TX; // flush transmit buffer
		NRD_SEND_CMD(cmd);
		NRF_WRITE_REGISTER(FIFO_STATUS, 0x11); // reset fifo
	}

	return 0;
}

uint8_t is_data_on_pipe(uint8_t pipenum)
{
	uint8_t status_reg = NRF_READ_REGISTER(STATUS);
	// if 6 th bit is set and respective data pipe is set
	if (status_reg & (NEW_DATA_ON_RX_FIFO))
	{
		// clear rx_dr
		NRF_WRITE_REGISTER(STATUS, (NEW_DATA_ON_RX_FIFO));
		return 1; // data received
	}
	return 0;
}

void NRF_RECV_DATA(uint8_t *data_ptr_RECV)
{
	uint8_t cmd = R_RX_PL_WID;
	uint8_t payLoad_width = 0;
	CSN_SELECT_NRF();
	cmd = R_RX_PAYLOAD; // send command to read the payload data
	SPI_TX_MULTI(&cmd, 1);
	SPI_READ_MULTI(data_ptr_RECV, 10); // fetch payload
	CSN_UNSELECT_NRF();
	delay(10);
	cmd = FLUSH_RX;
	NRD_SEND_CMD(cmd); // flush rx fifo
}
