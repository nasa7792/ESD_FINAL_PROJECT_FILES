/* ---------------------------------------------------------------------------------
 * Abhirath Koushik, Nalin Saxena and Husamuldeen (Embedded Expert IO) (https://blog.embeddedexpert.io/?p=1215)
 * ECEN 5613 - Fall 2024 - Prof. McClure
 * University of Colorado Boulder
 * Revised 12/15/2024
 *  --------------------------------------------------------------------------------
 * This file contains the Function Declarations for the Display Functions of the Graphical LCD Display.
   ---------------------------------------------------------------------------------*/

/* -------------------------------------------------- */
//          INCLUDES & DEFINES
/* -------------------------------------------------- */
#include <stdint.h>

/* -------------------------------------------------- */
//          FUNCTION DECLARATIONS
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
void LCD_initial_display(void);

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
void convert_to_str(uint8_t RxData[], int recv_width, char str[]);

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
void LCD_display_data(uint8_t RxData[]);

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
void ST7789_Init(void);
