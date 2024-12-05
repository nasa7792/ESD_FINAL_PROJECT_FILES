

#include "gps.h"



char GGA[100];
char RMC[100];

GPSSTRUCT gpsData;

int flagGGA = 0, flagRMC = 0;
char gpsBuffer [50];
int count = 0;

int VCCTimeout = 5000; // GGA or RMC will not be received if the VCC is not sufficient

void parse_gps_output(float *latitude, char *NS, float *longitude, char *EW)
{
	  count++;
	  if (Wait_for("GGA") == 1)
	  {
		  VCCTimeout = 5000;  // Reset the VCC Timeout indicating the GGA is being received

		  Copy_upto("*", GGA);
		  if (decodeGGA(GGA, &gpsData.ggastruct) == 0) flagGGA = 2;  // 2 indicates the data is valid
		  else flagGGA = 1;  // 1 indicates the data is invalid
	  }
	  else{
		  printf("timeout of gps return \n \r");
		  return;
	  }

	  if ((flagGGA == 2) | (flagRMC == 2))
	  {
		  memset(gpsBuffer, '\0', 50);

		  sprintf (gpsBuffer, "%.2f%c,%.2f%c  ", gpsData.ggastruct.lcation.latitude, gpsData.ggastruct.lcation.NS,\
				  gpsData.ggastruct.lcation.longitude, gpsData.ggastruct.lcation.EW);
		  printf("\n \r Gps data is %s \n \r",gpsBuffer);
		    // Use sscanf to extract the values
		  if (sscanf(gpsBuffer, "%f%c,%f%c", latitude, NS, longitude, EW) != 4) {
			printf("Error: Unable to parse the input string.\n");
		  }
	  }
}
