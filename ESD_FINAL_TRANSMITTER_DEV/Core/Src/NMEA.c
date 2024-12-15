/* ---------------------------------------------------------------------------------
 * Abhirath Koushik and Arun R (Controllers Tech)
 * ECEN 5613 - Fall 2024 - Prof. McClure
 * University of Colorado Boulder
 * Revised 12/14/24
 *  --------------------------------------------------------------------------------
 * This file contains the functions related to NMEA formatting required for GPS Functions.
   ---------------------------------------------------------------------------------*/

/* -------------------------------------------------- */
//          INCLUDES & DEFINES
/* -------------------------------------------------- */
#include "NMEA.h"
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"

/* -------------------------------------------------- */
//          INCLUDES & DEFINES
/* -------------------------------------------------- */
int inx = 0;

/* -------------------------------------------------- */
//          FUNCTION DEFINITIONS
/* -------------------------------------------------- */

/*
 * Function to parse the NMEA format to extract Latitude and Longitude information.
 * This code was originally written by Arun R of Controllers Tech, with modifications by Abhirath Koushik.
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
int decodeGGA (char *GGAbuffer, GGASTRUCT *gga)
{
	inx = 0;
	char buffer[12];
	int i = 0;
	while (GGAbuffer[inx] != ',') inx++;  // 1st ','
	inx++;
	while (GGAbuffer[inx] != ',') inx++;  // After time ','
	inx++;
	while (GGAbuffer[inx] != ',') inx++;  // after latitude ','
	inx++;
	while (GGAbuffer[inx] != ',') inx++;  // after NS ','
	inx++;
	while (GGAbuffer[inx] != ',') inx++;  // after longitude ','
	inx++;
	while (GGAbuffer[inx] != ',') inx++;  // after EW ','
	inx++;  // reached the character to identify the GPS Position Fix
	if ((GGAbuffer[inx] == '1') || (GGAbuffer[inx] == '2') || (GGAbuffer[inx] == '6'))   // 0 indicates no fix yet
	{
		gga->isfixValid = 1;   // fix available
		inx = 0;   // reset the index and extract information now
	}
	else
	{
		gga->isfixValid = 0;   // If the fix is not available
		return 1;  // return error
	}
	while (GGAbuffer[inx] != ',') inx++;  // 1st ','

	// Get Latitude Information
	inx++;   // Reach the first number in the latitude
	memset(buffer, '\0', 12);
	i=0;
	while (GGAbuffer[inx] != ',')
	{
		buffer[i] = GGAbuffer[inx];
		i++;
		inx++;
	}
	if (strlen(buffer) < 6) return 2;  // If the buffer length is not appropriate, return error
	int16_t num = (atoi(buffer));
	int j = 0;
	while (buffer[j] != '.') j++;
	j++;
	int declen = (strlen(buffer))-j;  // calculate the number of digit after decimal
	int dec = atoi ((char *) buffer+j);
	float lat = (num/100.0) + (dec/pow(10, (declen+2)));
	gga->lcation.latitude = lat;  // save the latitude data into the structure
	inx++;  
	gga->lcation.NS = GGAbuffer[inx];  // save the N/S into the structure


	// Get Longitude Information
	inx++;  // ',' after NS character
	inx++;  // Reach the first number in the longitude
	memset(buffer, '\0', 12);
	i=0;
	while (GGAbuffer[inx] != ',')
	{
		buffer[i] = GGAbuffer[inx];
		i++;
		inx++;
	}
	num = (atoi(buffer));
	j = 0;
	while (buffer[j] != '.') j++;
	j++;
	declen = (strlen(buffer))-j;  // calculate the number of digit after decimal
	dec = atoi ((char *) buffer+j);
	lat = (num/100.0) + (dec/pow(10, (declen+2)));
	gga->lcation.longitude = lat;  // save the longitude data into the structure
	inx++;
	gga->lcation.EW = GGAbuffer[inx];  // save the E/W into the structure

	return 0;
}
