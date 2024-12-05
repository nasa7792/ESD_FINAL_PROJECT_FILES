/*
 * gps.h
 *
 *  Created on: Dec 4, 2024
 *      Author: Asus
 */

#ifndef INC_GPS_H_
#define INC_GPS_H_

#include <stdio.h>
#include <string.h>
#include "uartRingBuffer.h"
#include "NMEA.h"

void parse_gps_output(float *latitude, char *NS, float *longitude, char *EW);


#endif /* INC_GPS_H_ */
