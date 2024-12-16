/*
 * status_leds_command_buttons.h
 *
 *  Created on: Dec 8, 2024
 *      Author: Nalin Saxena
 * 
 * This file contains function declarations related to setting up leds and command buttons
 */

#ifndef INC_STATUS_LEDS_COMMAND_BUTTONS_H_
#define INC_STATUS_LEDS_COMMAND_BUTTONS_H_

/* -------------------------------------------------- */
//          INCLUDES & DEFINES
/* -------------------------------------------------- */

#define GPS_ON_INDICATOR GPIO_BSRR_BS_8
#define GPS_OFF_INDICATOR GPIO_BSRR_BR_8

#define MAX30102_ON_INDICATOR GPIO_BSRR_BS_9
#define MAX30102_OFF_INDICATOR GPIO_BSRR_BR_9

#define DISABLE_GPS_COMMAND "DIS_GPS" //disable gps command
#define DISABLE_MAX_COMMAND "DIS_MAX" //isable max command
#define ACK_DEF_COMMAND		"ACK_DEF"

/* -------------------------------------------------- */
//          FUNCTION DECLARATIONS
/* -------------------------------------------------- */


/*
 * configure status leds as output
 * Parameters:
 * 			 none
 * Returns:
 * 			none
 */
void setup_enable_status_leds();

/*
 * configure command buttons as alternate functionality and interrupts
 * Parameters:
 * 			 none
 * Returns:
 * 			none
 */
void  command_button_config();


#endif /* INC_STATUS_LEDS_COMMAND_BUTTONS_H_ */
