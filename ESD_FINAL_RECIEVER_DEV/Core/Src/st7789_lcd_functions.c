/* ---------------------------------------------------------------------------------
 * Abhirath Koushik and Husamuldeen (Embedded Expert IO) (https://blog.embeddedexpert.io/?p=1215)
 * ECEN 5613 - Fall 2024 - Prof. McClure
 * University of Colorado Boulder
 * Revised 12/15/2024
 *  --------------------------------------------------------------------------------
 * This file contains the functions related to the ST7789V Graphical LCD Display.
   ---------------------------------------------------------------------------------*/

/* -------------------------------------------------- */
//          INCLUDES & DEFINES
/* -------------------------------------------------- */
#include <stm32f411xe.h>
#include "st7789_lcd_functions.h"
#include "SPI.h"
#include "delay.h"

#define USART_BAUDRATE 9600   // Defining the Baud Rate used as 9600
#define CLOCK 16000000        // Considering 16 MHz clock for APB2 (for USART1 and USART2)
#define LCD_WIDTH_VAL_1 40
#define LCD_HEIGHT_VAL_1 16
#define LCD_WIDTH_VAL_2 11
#define LCD_HEIGHT_VAL_2 18
#define LCD_WINDOW_MASK_1 8
#define LCD_WINDOW_MASK_2 0xFF
#define LCD_WRITE_MASK_1 32
#define LCD_WRITE_MASK_2 0x8000

/* -------------------------------------------------- */
//          GLOBALS
/* -------------------------------------------------- */
// Defined by Abhirath Koushik
const tFont Font_11x18 = {LCD_WIDTH_VAL_2, LCD_HEIGHT_VAL_2, Font11x18_array};
const tImage Earth_img = {earth_image, LCD_WIDTH_VAL_1, LCD_WIDTH_VAL_1, LCD_HEIGHT_VAL_1};
const tImage Spo2 = {spo2_image, LCD_WIDTH_VAL_1, LCD_WIDTH_VAL_1, LCD_HEIGHT_VAL_1};
const tImage thermo = {thermo_image, LCD_WIDTH_VAL_1, LCD_WIDTH_VAL_1, LCD_HEIGHT_VAL_1};

/*
 * Function for SPI Initialization of the LCD Module.
 * This code is written by Abhirath Koushik.
 *
 * Parameters:
 * 	None
 *
 * Returns:
 * 	None
 */
void LCD_spi_init(void)
{
    SPI_INIT(); // Calling the common SPI Initialization Function

    /* Configure DC, RST, CS, and BL pins */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // Enable GPIOB clock for the Pins
    GPIOB->MODER &= ~(GPIO_MODER_MODE5_Msk | GPIO_MODER_MODE6_Msk | GPIO_MODER_MODE7_Msk | GPIO_MODER_MODE4_Msk); // Clear the mode bits for the Pins
    GPIOB->MODER |= (GPIO_MODER_MODE5_0 | GPIO_MODER_MODE6_0 | GPIO_MODER_MODE7_0 | GPIO_MODER_MODE4_0); // Set the Pins to output mode
}

/*
 * Function for writing Commands to the LCD.
 * This code is written by Abhirath Koushik.
 *
 * Parameters:
 * 	cmd: Command to be executed
 *
 * Returns:
 * 	None
 */
void LCD_write_command(uint8_t cmd)
{
	GPIOB->BSRR = GPIOB_BSRR_RESET_DC; // DC Low indicating Command
	GPIOB->BSRR = GPIOB_BSRR_RESET_CS; // Chip Select Low
	SPI_TX_MULTI(&cmd,  1);
	GPIOB->BSRR = GPIOB_BSRR_SET_CS;   // Chip Select High
}

/*
 * Function for writing Data to the LCD of any given size.
 * This code is written by Abhirath Koushik.
 *
 * Parameters:
 * 	buff:      Pointer to the buffer which stores the data to be written
 * 	buff_size: Size of the data to be written
 *
 * Returns:
 * 	None
 */
void LCD_write_data(uint8_t *buff, uint32_t buff_size)
{
	GPIOB->BSRR = GPIOB_BSRR_SET_DC;   // DC High indicating Data
	GPIOB->BSRR = GPIOB_BSRR_RESET_CS; // Chip Select Low
	SPI_TX_MULTI(buff, buff_size);
	GPIOB->BSRR = GPIOB_BSRR_SET_CS;   // Chip Select High
}

/*
* Simplified function for writing 1 byte of data to the LCD.
* This code is written by Abhirath Koushik.
*
* Parameters:
* 	data: Data to be written
*
* Returns:
* 	None
*/
void LCD_write_byte_data(uint8_t data)
{
	GPIOB->BSRR = GPIOB_BSRR_SET_DC;   // DC High indicating Data
	GPIOB->BSRR = GPIOB_BSRR_RESET_CS; // Chip Select Low
	SPI_TX_MULTI(&data, 1);
	GPIOB->BSRR = GPIOB_BSRR_SET_CS;   // Chip Select High
}

/*
* Function to Set the Address Window in the LCD.
* This code is written by Abhirath Koushik.
*
* Parameters:
* 	data: Data to be written
*
* Returns:
* 	None
*/
void LCD_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    uint16_t x_start = x0;
    uint16_t x_end = x1;
    uint16_t y_start = y0;
    uint16_t y_end = y1;

    // Set column address
    LCD_write_command(ST7789_CASET); // Column Address Set command
    uint8_t col_data[] = {x_start >> LCD_WINDOW_MASK_1, x_start & LCD_WINDOW_MASK_2,
    		x_end >> LCD_WINDOW_MASK_1, x_end & LCD_WINDOW_MASK_2};
    LCD_write_data(col_data, sizeof(col_data));

    // Set row address
    LCD_write_command(ST7789_RASET); // Row Address Set command
    uint8_t row_data[] = {y_start >> LCD_WINDOW_MASK_1, y_start & LCD_WINDOW_MASK_2,
    		y_end >> LCD_WINDOW_MASK_1, y_end & LCD_WINDOW_MASK_2};
    LCD_write_data(row_data, sizeof(row_data));

    LCD_write_command(ST7789_RAMWR); // RAM Write command
}

/*
* Function to Fill Color to the Whole LCD Screen.
* This code is written by Abhirath Koushik.
*
* Parameters:
* 	color: Color code (in hex)
*
* Returns:
* 	None
*/
void LCD_fill_screen_color(uint16_t color)
{
	LCD_address_window(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
	for (uint16_t i = 0; i < LCD_WIDTH; i++){
		for (uint16_t j = 0; j < LCD_HEIGHT; j++){
			uint8_t data[] = {color >> LCD_WINDOW_MASK_1, color & LCD_WINDOW_MASK_2};
			LCD_write_data(data, sizeof(data));
		}
	}
}

/*
* Function to draw a pixel at a particular position on the LCD with color.
* This code is written by Abhirath Koushik.
*
* Parameters:
*   x      : Column Position on the LCD
*   y      : Row Position on the LCD
* 	color  : Color code (in hex)
*
* Returns:
* 	None
*/
void LCD_draw_pixel(uint16_t x, uint16_t y, uint16_t color)
{
	LCD_address_window(x, y, x, y); // Set the same boundaries of x and y for a pixel
	uint8_t data[] = {color >> LCD_WINDOW_MASK_1, color & LCD_WINDOW_MASK_2};
	LCD_write_data(data, sizeof(data));
}

/*
* Function to draw an image at a particular position on the LCD with color.
* This code is written by Abhirath Koushik.
*
* Parameters:
*   x      : Start Column Position on the LCD
*   y      : Start Row Position on the LCD
*   width  : Width of the Image on the LCD
*   height : Height of the Image on the LCD
*   img_bit_map : Bitmap Array of the Image to be displayed
*
* Returns:
* 	None
*/
void LCD_draw_image(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t *img_bit_map)
{
    for (uint16_t row = 0; row < height; row++) {
        for (uint16_t col = 0; col < width; col++) {
            uint16_t color = img_bit_map[row * width + col];
            LCD_draw_pixel(x + col, y + row, color);
        }
    }
}

/*
* Function to write a character at a particular position on the LCD,
* along with its text color and background color.
* This code was originally written by Husamuldeen (Embedded Expert IO) and modified by Abhirath Koushik.
*
* Parameters:
*   x      : Start Column Position on the LCD
*   y      : Start Row Position on the LCD
*   ch     : Character to write
*   font   : Bitmap Array of the Font to use
*   color  : Text Color of the Character
*   bgcolor: Background Color around the boundaries of the character
*
* Returns:
* 	None
*/
void LCD_write_character(uint16_t x, uint16_t y, char ch, tFont font, uint16_t color, uint16_t bgcolor)
{
    uint32_t i, b, j;

    // Set the address window for the character display
    LCD_address_window(x, y, x + font.width - 1, y + font.height - 1);

    // Loop through each row of the character's font data
    for (i = 0; i < font.height; i++) {
        // Retrieve the bitmap data for the current row of the character
        b = font.data[(ch - LCD_WRITE_MASK_1) * font.height + i];

        // Loop through each pixel in the current row
        for (j = 0; j < font.width; j++) {
            // Check if the current pixel is part of the character foreground or the background
            if ((b << j) & LCD_WRITE_MASK_2) {
                // Foreground pixel: set to the specified color
                uint8_t data[] = {color >> LCD_WINDOW_MASK_1, color & LCD_WINDOW_MASK_2};
                LCD_write_data(data, sizeof(data));
            } else {
                // Background pixel: set to the specified background color
                uint8_t data[] = {bgcolor >> LCD_WINDOW_MASK_1, bgcolor & LCD_WINDOW_MASK_2};
                LCD_write_data(data, sizeof(data));
            }
        }
    }
}

/*
* Function to write a string at a particular position on the LCD,
* along with its text color and background color.
* This code was originally written by Husamuldeen (Embedded Expert IO) and modified by Abhirath Koushik.
*
* Parameters:
*   x      : Start Column Position on the LCD
*   y      : Start Row Position on the LCD
*   ch     : Character to write
*   font   : Bitmap Array of the Font to use
*   color  : Text Color of the Character
*   bgcolor: Background Color around the boundaries of the character
*
* Returns:
* 	None
*/
void LCD_write_string(uint16_t x, uint16_t y, const char *str, tFont font, uint16_t color, uint16_t bgcolor)
{
    // Loop through each character in the string
    while (*str) {
        // Move to next line if character exceeds screen width
        if (x + font.width >= LCD_WIDTH) {
            x = 0; // Reset x-coordinate
            y += font.height; // Move y-coordinate down by one character height

            // Check if the new line will exceed the screen height
            if (y + font.height >= LCD_HEIGHT) {
                // If there's no more vertical space, stop drawing
                break;
            }

            // Skip the character if space is detected
            if (*str == ' ') {
                str++;
                continue;
            }
        }

        // Call the Write Character function to write the individual characters on the LCD
        LCD_write_character(x, y, *str, font, color, bgcolor);

        // Move x-coordinate forward for the next character
        x += font.width;

        // Advance to the next character in the string
        str++;
    }
}
