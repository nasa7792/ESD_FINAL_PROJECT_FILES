/* ---------------------------------------------------------------------------------
 * Abhirath Koushik and Arun R (Controllers Tech)
 * ECEN 5613 - Fall 2024 - Prof. McClure
 * University of Colorado Boulder
 * Revised 12/14/24
 *  --------------------------------------------------------------------------------
 * This file contains function declarations related to NMEA formatting required for GPS Functions.
   ---------------------------------------------------------------------------------*/

#ifndef INC_NMEA_H_
#define INC_NMEA_H_

/* -------------------------------------------------- */
//          GLOBALS
/* -------------------------------------------------- */
// These structures are defined by Arun R (Controllers Tech)
typedef struct {
	float latitude;
	char NS;
	float longitude;
	char EW;
}LOCATION;

typedef struct {
	LOCATION lcation;
	int isfixValid;
}GGASTRUCT;

typedef struct {
	GGASTRUCT ggastruct;
}GPSSTRUCT;

/* -------------------------------------------------- */
//          FUNCTION DECLARATIONS
/* -------------------------------------------------- */

/*
 * Function to parse the NMEA format to extract Latitude and Longitude information.
 *
 * Parameters:
 * 	GGAbuffer : Input buffer containing the GGA sentence
 * 	gga       : Pointer to the structure where parsed data is stored.
 *
 * Returns:
 * 	0: Success
 * 	1: Error in GPS Fix
 * 	2: Error in Buffer Length for storing the data
 */
int decodeGGA (char *GGAbuffer, GGASTRUCT *gga);

#endif /* INC_NMEA_H_ */
