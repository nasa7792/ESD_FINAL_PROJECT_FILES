/*******************************************************************************
* Copyright (C) 2024 by Abhirath Koushik
*
* Redistribution, modification or use of this software in source or binary
* forms is permitted as long as the files maintain this copyright. Users are
* permitted to modify this and use it to learn about the field of embedded
* software. Abhirath Koushik and the University of Colorado are not liable for
* any misuse of this material.
* ****************************************************************************/

/**
* @file    st7789_lcd_functions.c
* @brief   This file contains the functions related to ST7789V LCD Functions.
*
*
* @author  Abhirath Koushik
* @date    12-04-2024
* @change_history Abhirath Koushik: Initial File Creation
*/

#include<stm32f411xe.h>
#include "st7789_lcd_functions.h"
#include "SPI.h"
#include "delay.h"

#define USART_BAUDRATE 9600
#define CLOCK 16000000  // Assume 16 MHz clock for APB2 (for USART1 and USART2)



const FontDef Font_11x18 = {11, 18, Font11x18_array};


const tImage Earth_img = { EARTH_IMAGE, 40, 40,
    16 };

const tImage Spo2 = { spo2_image, 40, 40,
    16 };

const tImage thermo = { THERMO_IMAGE, 40, 40,
    16 };


void st7789_spi_init()
{
    SPI_INIT();

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // Enable GPIOB clock for Pins PB4 (BL), PB5 (DC), PB6 (CS), and PB7 (RST)

    /* Configure DC, RST, CS, and BL pins */
    GPIOB->MODER &= ~(GPIO_MODER_MODE5_Msk | GPIO_MODER_MODE6_Msk | GPIO_MODER_MODE7_Msk | GPIO_MODER_MODE4_Msk);
    // Clear the mode bits for PB4, PB5, PB6, and PB7

    GPIOB->MODER |= (GPIO_MODER_MODE5_0 | GPIO_MODER_MODE6_0 | GPIO_MODER_MODE7_0 | GPIO_MODER_MODE4_0);
    // Set PB4, PB5, PB6, and PB7 to output mode (01)
}

void lcd_initial_characters(void)
{
	uint8_t rotate=0;
	rotate=0x60;
	ST7789_WriteCommand(ST7789_MADCTL);
	ST7789_WriteSmallData(rotate);
	ST7789_WriteString(20,20, "Wireless Sensor      Communication System", Font_11x18, GREEN, LIGHT_BLUE);
	ST7789_DrawImage(20,80,40,40,THERMO_IMAGE);
	ST7789_DrawImage(20,130,40,40,spo2_image);
	ST7789_DrawImage(20,180,40,40,EARTH_IMAGE);
}

void ST7789_WriteCommand(uint8_t cmd)
{
	GPIOB->BSRR = GPIOB_BSRR_RESET_DC; // DC Low indicating Command
	GPIOB->BSRR = GPIOB_BSRR_RESET_CS; // Chip Select Low
	SPI_TX_MULTI(&cmd,  1);
	GPIOB->BSRR = GPIOB_BSRR_SET_CS; // Chip Select High
}

 void ST7789_WriteData(uint8_t *buff, uint32_t buff_size)
{
	GPIOB->BSRR = GPIOB_BSRR_SET_DC;
	GPIOB->BSRR = GPIOB_BSRR_RESET_CS; // Chip Select Low
	SPI_TX_MULTI(buff, buff_size);
	GPIOB->BSRR = GPIOB_BSRR_SET_CS; // Chip Select High
}

 void ST7789_WriteSmallData(uint8_t data)
{
	GPIOB->BSRR = GPIOB_BSRR_SET_DC; // DC High indicating Data
	GPIOB->BSRR = GPIOB_BSRR_RESET_CS; // Chip Select Low
	SPI_TX_MULTI(&data, 1);
	GPIOB->BSRR = GPIOB_BSRR_SET_CS; // Chip Select High
}

 void ST7789_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	uint16_t x_start = x0 + X_SHIFT, x_end = x1 + X_SHIFT;
	uint16_t y_start = y0 + Y_SHIFT, y_end = y1 + Y_SHIFT;

	/* Column Address set */
	ST7789_WriteCommand(ST7789_CASET);
	{
		uint8_t data[] = {x_start >> 8, x_start & 0xFF, x_end >> 8, x_end & 0xFF};
		ST7789_WriteData(data, sizeof(data));
	}

	/* Row Address set */
	ST7789_WriteCommand(ST7789_RASET);
	{
		uint8_t data[] = {y_start >> 8, y_start & 0xFF, y_end >> 8, y_end & 0xFF};
		ST7789_WriteData(data, sizeof(data));
	}
	/* Write to RAM */
	ST7789_WriteCommand(ST7789_RAMWR);
}

void ST7789_Fill_Color(uint16_t color)
{
	uint16_t i;
	ST7789_SetAddressWindow(0, 0, ST7789_WIDTH - 1, ST7789_HEIGHT - 1);

	uint16_t j;
	for (i = 0; i < ST7789_WIDTH; i++)
			for (j = 0; j < ST7789_HEIGHT; j++) {
				uint8_t data[] = {color >> 8, color & 0xFF};
				ST7789_WriteData(data, sizeof(data));
			}

}

void ST7789_WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor)
{
	uint32_t i, b, j;

	ST7789_SetAddressWindow(x, y, x + font.width - 1, y + font.height - 1);

	for (i = 0; i < font.height; i++) {
		b = font.data[(ch - 32) * font.height + i];
		for (j = 0; j < font.width; j++) {
			if ((b << j) & 0x8000) {
				uint8_t data[] = {color >> 8, color & 0xFF};
				ST7789_WriteData(data, sizeof(data));
			}
			else {
				uint8_t data[] = {bgcolor >> 8, bgcolor & 0xFF};
				ST7789_WriteData(data, sizeof(data));
			}
		}
	}

}

void ST7789_WriteString(uint16_t x, uint16_t y, const char *str, FontDef font, uint16_t color, uint16_t bgcolor)
{

	while (*str) {
		if (x + font.width >= ST7789_WIDTH) {
			x = 0;
			y += font.height;
			if (y + font.height >= ST7789_HEIGHT) {
				break;
			}

			if (*str == ' ') {
				// skip spaces in the beginning of the new line
				str++;
				continue;
			}
		}
		ST7789_WriteChar(x, y, *str, font, color, bgcolor);
		x += font.width;
		str++;
	}

}

/**
 * @brief Initialize ST7789 controller
 * @param none
 * @return none
 */
void ST7789_Init(void)
{
	st7789_spi_init();
	delay(25);
	GPIOB->BSRR = GPIOB_BSRR_RESET_RST;
	delay(50);
	GPIOB->BSRR = GPIOB_BSRR_SET_RST;
    delay(50);
    ST7789_WriteCommand(ST7789_SWRESET);
    delay(100);
    ST7789_WriteCommand(ST7789_COLMOD);		//	Set color mode
    ST7789_WriteSmallData(ST7789_COLOR_MODE_16bit);
  	ST7789_WriteCommand(0xB2);				//	Porch control
	uint8_t data[] = {0x0C, 0x0C, 0x00, 0x33, 0x33};
	ST7789_WriteData(data, sizeof(data));

	/* Internal LCD Voltage generator settings */
    ST7789_WriteCommand(0XB7);				//	Gate Control
    ST7789_WriteSmallData(0x35);			//	Default value
    ST7789_WriteCommand(0xBB);				//	VCOM setting
    ST7789_WriteSmallData(0x20);			//	0.725v (default 0.75v for 0x20)
    ST7789_WriteCommand(0xC0);				//	LCMCTRL
    ST7789_WriteSmallData (0x2C);			//	Default value
    ST7789_WriteCommand (0xC2);				//	VDV and VRH command Enable
    ST7789_WriteSmallData (0x01);			//	Default value
    ST7789_WriteCommand (0xC3);				//	VRH set
    ST7789_WriteSmallData (0x0b);			//	+-4.45v (defalut +-4.1v for 0x0B)
    ST7789_WriteCommand (0xC4);				//	VDV set
    ST7789_WriteSmallData (0x20);			//	Default value
    ST7789_WriteCommand (0xC6);				//	Frame rate control in normal mode
    ST7789_WriteSmallData (0x0F);			//	Default value (60HZ)
    ST7789_WriteCommand (0xD0);				//	Power control
    ST7789_WriteSmallData (0xA4);			//	Default value
    ST7789_WriteSmallData (0xA1);			//	Default value
	/**************** Division line ****************/

	ST7789_WriteCommand(0xE0);
	{
		uint8_t data[] = {0xD0, 0x04, 0x0D, 0x11, 0x13, 0x2B, 0x3F, 0x54, 0x4C, 0x18, 0x0D, 0x0B, 0x1F, 0x23};
		ST7789_WriteData(data, sizeof(data));
	}

    ST7789_WriteCommand(0xE1);
	{
		uint8_t data[] = {0xD0, 0x04, 0x0C, 0x11, 0x13, 0x2C, 0x3F, 0x44, 0x51, 0x2F, 0x1F, 0x1F, 0x20, 0x23};
		ST7789_WriteData(data, sizeof(data));
	}
    ST7789_WriteCommand (ST7789_INVON);		//	Inversion ON
	ST7789_WriteCommand (ST7789_SLPOUT);	//	Out of sleep mode
  	ST7789_WriteCommand (ST7789_NORON);		//	Normal Display on
  	ST7789_WriteCommand (ST7789_DISPON);	//	Main screen turned on

  	delay(120);

  	GPIOB->BSRR = GPIOB_BSRR_SET_PB4;  // Set PB4 high

  	delay(50);
	ST7789_Fill_Color(LIGHT_BLUE);
}

void ST7789_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
	if ((x < 0) || (x >= ST7789_WIDTH) ||
		 (y < 0) || (y >= ST7789_HEIGHT))	return;

	ST7789_SetAddressWindow(x, y, x, y);
	uint8_t data[] = {color >> 8, color & 0xFF};
	ST7789_WriteData(data, sizeof(data));
}


void ST7789_DrawImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t *imageData)
{
    for (uint16_t row = 0; row < height; row++) {
        for (uint16_t col = 0; col < width; col++) {
            uint16_t color = imageData[row * width + col];
            ST7789_DrawPixel(x + col, y + row, color);
        }
    }
}

void convert_to_str(uint8_t RxData[],int recv_width,char str[])
{
	int i=0;
	while(recv_width--){
		str[i]=RxData[i];
		i++;
	}
	str[i]='\0';
}

