/* ---------------------------------------------------------------------------------
 * Abhirath Koushik
 * ECEN 5613 - Fall 2024 - Prof. McClure
 * University of Colorado Boulder
 * Revised 12/14/24
 *  --------------------------------------------------------------------------------
 * This file contains the functions related to obtaining GPS Data.
   ---------------------------------------------------------------------------------*/

/* -------------------------------------------------- */
//          INCLUDES & DEFINES
/* -------------------------------------------------- */
#include <stdbool.h>
#include "gps.h"
#include "formated_printf.h"

#define timeout_value 5000
#define buffer_value 50
#define info_value 4

/* -------------------------------------------------- */
//          GLOBALS
/* -------------------------------------------------- */
GPSSTRUCT gpsData;
char GGA[buffer_value * 2];
int flagGGA = 0;
char gpsBuffer [buffer_value];
int count = 0;
int VCCTimeout = timeout_value; // GGA will get timeout based on this counter value

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
void parse_gps_output(float *latitude, char *NS, float *longitude, char *EW)
{
	  count++;
	  if (Wait_for("GGA") == 1)
	  {
		  VCCTimeout = timeout_value;  // Reset the VCC Timeout once we know GGA is being received
		  Copy_upto("*", GGA);
		  // Call the decodeGGA function to obtain the GPS information
		  if (decodeGGA(GGA, &gpsData.ggastruct) == 0) flagGGA = true;
		  else flagGGA = false;
	  } else {
		  print_error("TIMEOUT of GPS!\n\r");
		  return;
	  }

	  if (flagGGA == true)
	  {
		  memset(gpsBuffer, '\0', buffer_value);
		  sprintf (gpsBuffer, "%.2f%c,%.2f%c  ", gpsData.ggastruct.lcation.latitude, gpsData.ggastruct.lcation.NS,\
				  gpsData.ggastruct.lcation.longitude, gpsData.ggastruct.lcation.EW);
		  print_success("\n\rGPS DATA SUCCESS");
		  print_success(gpsBuffer);
		    // Use sscanf to extract the values
		  if (sscanf(gpsBuffer, "%f%c,%f%c", latitude, NS, longitude, EW) != info_value) {
			  print_error("Error: Unable to parse the input string.\n");
		  }
	  }
}
