/* ---------------------------------------------------------------------------------
 * Abhirath Koushik
 * ECEN 5613 - Fall 2024 - Prof. McClure
 * University of Colorado Boulder
 * Revised 12/14/24
 *  --------------------------------------------------------------------------------
 * This file contains function declarations related to GPS Parsing Functions.
   ---------------------------------------------------------------------------------*/

#ifndef INC_GPS_H_
#define INC_GPS_H_

/* -------------------------------------------------- */
//          INCLUDES & DEFINES
/* -------------------------------------------------- */
#include <stdio.h>
#include <string.h>
#include "uartRingBuffer.h"
#include "NMEA.h"

/* -------------------------------------------------- */
//          FUNCTION DEFINITIONS
/* -------------------------------------------------- */

/*
 * Function to parse the GPS Data for Latitude and Longitude coordinates.
 *
 * Parameters:
 * 	latitude : Latitude Values of a Location
 * 	NS       : Indicates North or South direction for the Latitude
 * 	longitude: Longitude Values of a Location
 * 	EW       : Indicates East or West direction for the Longitude
 *
 * Returns:
 * 	None
 */
void parse_gps_output(float *latitude, char *NS, float *longitude, char *EW);

#endif /* INC_GPS_H_ */
