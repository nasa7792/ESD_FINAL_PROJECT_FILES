/* ---------------------------------------------------------------------------------
 * Abhirath Koushik
 * ECEN 5613 - Fall 2024 - Prof. McClure
 * University of Colorado Boulder
 * Revised 12/15/24
 *  --------------------------------------------------------------------------------
 * This file contains function declarations related to SD Card Processing Functions.
   ---------------------------------------------------------------------------------*/

/* -------------------------------------------------- */
//          INCLUDES & DEFINES
/* -------------------------------------------------- */
#include "sd_card.h"
#include "fatfs.h"
#include"formated_printf.h"

/* -------------------------------------------------- */
//          FUNCTION DEFINITIONS
/* -------------------------------------------------- */

/*
 * Function to log the payload information to the SD Card.
 *
 * Parameters:
 * 	log_str : Character Array to store the information to be written to the SD Card
 *
 * Returns:
 * 	None
 */
void process_SD_card(char log_str[])
{
  FATFS       Fat_handler;                // FATFs Handler
  FIL         file_var;                   // File  Handle
  FRESULT     file_result;                // Result Flag after Operations

  do
  {
    // Mount the SD Card
	file_result = f_mount(&Fat_handler, "", 1);
    if (file_result != FR_OK)
    {
      printf("No SD Card found : (%i)\r\n", file_result);
      break;
    }
    print_success("\n\rSD Card Mounted Successfully!\r\n");

    // Open the File in Append Mode to Write Data
    file_result = f_open(&file_var, "logger.txt", FA_OPEN_APPEND| FA_WRITE);
    if(file_result != FR_OK)
    {
      printf("File creation/open Error : (%i)\r\n", file_result);
      break;
    }

    printf("\n\rWriting data to logger.txt!\r\n");
    strcat(log_str, "\n\r"); // Adding \n\r to add the data to new line in the file

    // Write the data into the text file
    f_puts(log_str, &file_var);

    // Close the text file after adding the data
    f_close(&file_var);
  } while(false);

  // Unmount the SD Card
  f_mount(NULL, "", 0);
  print_success("\n\rSD Card Unmounted Successfully!\r\n");
}
