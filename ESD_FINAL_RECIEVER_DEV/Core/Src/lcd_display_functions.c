/* ---------------------------------------------------------------------------------
 * Abhirath Koushik, Nalin Saxena and Husamuldeen (Embedded Expert IO) (https://blog.embeddedexpert.io/?p=1215)
 * ECEN 5613 - Fall 2024 - Prof. McClure
 * University of Colorado Boulder
 * Revised 12/15/2024
 *  --------------------------------------------------------------------------------
 * This file contains the Display Functions of the Graphical LCD Display.
   ---------------------------------------------------------------------------------*/

/* -------------------------------------------------- */
//          INCLUDES & DEFINES
/* -------------------------------------------------- */
#include <string.h>
#include "lcd_display_functions.h"
#include "st7789_lcd_functions.h"
#include "NRF_DRIVER.h"
#include "formated_printf.h"
#include "delay.h"

/* -------------------------------------------------- */
//          GLOBALS
/* -------------------------------------------------- */
// Defined by Abhirath Koushik
#define LCD_ROTATE_VALUE 0x60
#define LCD_IMAGE_SIZE 40

#define LCD_ROW_VAL_1 20
#define LCD_ROW_VAL_2 80
#define LCD_ROW_VAL_3 130
#define LCD_ROW_VAL_4 180
#define LCD_ROW_VAL_5 90
#define LCD_ROW_VAL_6 130
#define LCD_ROW_VAL_7 170
#define LCD_ROW_VAL_8 190

#define LCD_COLUMN_VAL_1 20
#define LCD_COLUMN_VAL_2 100

/* -------------------------------------------------- */
//          FUNCTION DEFINITIONS
/* -------------------------------------------------- */

/*
 * Function for displaying the Initial Text and Icons for Displaying
 * Temperature, SpO2 and GPS Values.
 * This code is written by Nalin Saxena with minor modifications by Abhirath Koushik.
 *
 * Parameters:
 * 	None
 *
 * Returns:
 * 	None
 */
void LCD_initial_display(void)
{
	uint8_t rotate = LCD_ROTATE_VALUE;
	LCD_write_command(ST7789_MADCTL);
	LCD_write_byte_data(rotate);
	LCD_write_string(LCD_COLUMN_VAL_1, LCD_ROW_VAL_1, "Wireless Sensor      Communication System", Font_11x18, GREEN, LIGHT_BLUE);
	LCD_draw_image(LCD_COLUMN_VAL_1, LCD_ROW_VAL_2, LCD_IMAGE_SIZE, LCD_IMAGE_SIZE, thermo_image);
	LCD_draw_image(LCD_COLUMN_VAL_1, LCD_ROW_VAL_3, LCD_IMAGE_SIZE, LCD_IMAGE_SIZE, spo2_image);
	LCD_draw_image(LCD_COLUMN_VAL_1, LCD_ROW_VAL_4, LCD_IMAGE_SIZE, LCD_IMAGE_SIZE, earth_image);
}

/*
 * Function to convert uint8 (from payload) to string to display on the LCD.
 * This code is written by Nalin Saxena.
 *
 * Parameters:
 * 	RxData: Array with the payload information
 * 	recv_width: Size of the Array
 * 	str: Array to store the converted string data
 *
 * Returns:
 * 	None
 */
void convert_to_str(uint8_t RxData[], int recv_width, char str[])
{
	int i=0;
	while(recv_width--){
		str[i]=RxData[i];
		i++;
	}
	str[i]='\0';
}

/*
 * Function to parse the received payload and display on the LCD.
 * Code for parsing the data is written by Nalin Saxena.
 * Code for displaying on the LCD is written by Abhirath Koushik.
 *
 * Parameters:
 * 	None
 *
 * Returns:
 * 	None
 */
void LCD_display_data(uint8_t RxData[])
{
	if(is_data_on_pipe(0)== 1){
		print_info("\n\rReceived Data:");
		uint8_t recv_width = NRF_RECV_DATA(RxData);
		print(RxData, recv_width);
		char str[32];
		convert_to_str(RxData, recv_width,str);
		char *delimeter="-";
		char* tok=strtok(str,delimeter);
		char heart_rate_data[10]={0};
		char spo2_data[10]={0};
		char gps_data_lat[10]={0};
		char gps_data_long[10]={0};
		int count_tok=0;

		while(tok != NULL){
			count_tok++;
			if(count_tok==1 && tok[0]=='T'){
				memset(heart_rate_data,0,10);
				strcpy(heart_rate_data,tok+1);
				printf("\033[32m\n\rTemperature Value: %s\033[0m\n\r", heart_rate_data);
			}

			else if(count_tok==2 && tok[0]=='O'){
				memset(spo2_data,0,10);
				strcpy(spo2_data,tok+1);
				printf("\033[32mSpO2  \t: %s\033[0m\n\r", spo2_data);
			}

			else if(count_tok==3 && tok[0]=='G'){
				char *delim=",";
				char* tok2=strtok(tok+1,delim);
				strcpy(gps_data_lat,tok2);
				tok2=strtok(NULL,delim);
				strcpy(gps_data_long,tok2);
				printf("\033[32mGPS Latitude: %s \n\rGPS Longitude: %s\033[0m\n\r", gps_data_lat, gps_data_long);
			}
			tok=strtok(NULL,delimeter);

		}
		//first clear previous text
		char text[] = "    ";
		LCD_write_string(LCD_COLUMN_VAL_2, LCD_ROW_VAL_5, text, Font_11x18, LIGHT_BLUE, LIGHT_BLUE); // Display Data on LCD
		LCD_write_string(LCD_COLUMN_VAL_2, LCD_ROW_VAL_6, text, Font_11x18, LIGHT_BLUE, LIGHT_BLUE); // Display Data on LCD
		LCD_write_string(LCD_COLUMN_VAL_2, LCD_ROW_VAL_7, text, Font_11x18, LIGHT_BLUE, LIGHT_BLUE); // Display Data on LCD
		LCD_write_string(LCD_COLUMN_VAL_2, LCD_ROW_VAL_8, text, Font_11x18, LIGHT_BLUE, LIGHT_BLUE); // Display Data on LCD

		LCD_write_string(LCD_COLUMN_VAL_2, LCD_ROW_VAL_5, heart_rate_data, Font_11x18, WHITE, LIGHT_BLUE); // Display Data on LCD
		LCD_write_string(LCD_COLUMN_VAL_2, LCD_ROW_VAL_6, spo2_data, Font_11x18, WHITE, LIGHT_BLUE); // Display Data on LCD
		LCD_write_string(LCD_COLUMN_VAL_2, LCD_ROW_VAL_7, gps_data_lat, Font_11x18, WHITE, LIGHT_BLUE); // Display Data on LCD
		LCD_write_string(LCD_COLUMN_VAL_2, LCD_ROW_VAL_8, gps_data_long, Font_11x18, WHITE, LIGHT_BLUE); // Display Data on LCD
	}
}

/*
 * Function with the Initialization Sequence for ST7789V LCD Controller.
 * This code was originally written by Husamuldeen (Embedded Expert IO), with modifications by Abhirath Koushik.
 *
 * Parameters:
 * 	None
 *
 * Returns:
 * 	None
 */
void ST7789_Init(void)
{
	LCD_spi_init(); // Call the LCD SPI Initialization Function
	delay(25);
	GPIOB->BSRR = GPIOB_BSRR_RESET_RST; // Reset Pin Low
	delay(50);
	GPIOB->BSRR = GPIOB_BSRR_SET_RST;   // Reset Pin High
    delay(50);
    LCD_write_command(ST7789_SWRESET); // Writing to the Reset Register
    delay(100);
    LCD_write_command(ST7789_COLMOD);		//	Set color mode
    LCD_write_byte_data(ST7789_COLOR_MODE_16bit);
    LCD_write_command(0xB2);				//	Porch control
	uint8_t data[] = {0x0C, 0x0C, 0x00, 0x33, 0x33};
	LCD_write_data(data, sizeof(data));

	/* Internal LCD Voltage generator settings */
	LCD_write_command(0XB7);				//	Gate Control
    LCD_write_byte_data(0x35);			//	Default value
    LCD_write_command(0xBB);				//	VCOM setting
    LCD_write_byte_data(0x20);			//	0.725v (default 0.75v for 0x20)
    LCD_write_command(0xC0);				//	LCMCTRL
    LCD_write_byte_data (0x2C);			//	Default value
    LCD_write_command (0xC2);				//	VDV and VRH command Enable
    LCD_write_byte_data (0x01);			//	Default value
    LCD_write_command (0xC3);				//	VRH set
    LCD_write_byte_data (0x0b);			//	+-4.45v (defalut +-4.1v for 0x0B)
    LCD_write_command (0xC4);				//	VDV set
    LCD_write_byte_data (0x20);			//	Default value
    LCD_write_command (0xC6);				//	Frame rate control in normal mode
    LCD_write_byte_data (0x0F);			//	Default value (60HZ)
    LCD_write_command (0xD0);				//	Power control
    LCD_write_byte_data (0xA4);			//	Default value
    LCD_write_byte_data (0xA1);			//	Default value

	LCD_write_command(0xE0);
	uint8_t data1[] = {0xD0, 0x04, 0x0D, 0x11, 0x13, 0x2B, 0x3F, 0x54, 0x4C, 0x18, 0x0D, 0x0B, 0x1F, 0x23};
	LCD_write_data(data1, sizeof(data1));
    LCD_write_command(0xE1);
	uint8_t data2[] = {0xD0, 0x04, 0x0C, 0x11, 0x13, 0x2C, 0x3F, 0x44, 0x51, 0x2F, 0x1F, 0x1F, 0x20, 0x23};
	LCD_write_data(data2, sizeof(data2));

    LCD_write_command (ST7789_INVON);		//	Inversion ON
	LCD_write_command (ST7789_SLPOUT);	//	Out of sleep mode
  	LCD_write_command (ST7789_NORON);		//	Normal Display on
  	LCD_write_command (ST7789_DISPON);	//	Main screen turned on

  	delay(120);
  	GPIOB->BSRR = GPIOB_BSRR_SET_PB4;  // Set Backlight Pin On
  	delay(50);
  	LCD_fill_screen_color(LIGHT_BLUE); // Fill the LCD screen with Light Blue Color
}
