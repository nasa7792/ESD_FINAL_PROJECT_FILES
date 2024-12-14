

#include "sd_card.h"
#include "fatfs.h"
#include"formated_printf.h"

void process_SD_card(char log_str[])
{
  FATFS       FatFs;                //Fatfs handle
  FIL         fil;                  //File handle
  FRESULT     fres;                 //Result after operations
  char        buf[100];

  do
  {
    //Mount the SD Card
    fres = f_mount(&FatFs, "", 1);    //1=mount now
    if (fres != FR_OK)
    {
      printf("No SD Card found : (%i)\r\n", fres);
      break;
    }
    print_success("SD Card Mounted Successfully!\r\n");

    //Read the SD Card Total size and Free Size
    FATFS *pfs;
    DWORD fre_clust;
    uint32_t totalSpace, freeSpace;

    f_getfree("", &fre_clust, &pfs);
    totalSpace = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
    freeSpace = (uint32_t)(fre_clust * pfs->csize * 0.5);

    //Open the file
    //FA_OPEN_APPEND  | FA_WRITE
    fres = f_open(&fil, "logger.txt", FA_OPEN_APPEND| FA_WRITE);
    if(fres != FR_OK)
    {
      printf("File creation/open Error : (%i)\r\n", fres);
      break;
    }

    printf("\n\rWriting data to logger.txt!\r\n");
    strcat(log_str, "\n\r");

    //write the data
    f_puts(log_str,&fil);

    //close your file
    f_close(&fil);

    //Open the file
    fres = f_open(&fil, "logger.txt", FA_READ);
    if(fres != FR_OK)
    {
      printf("File opening Error : (%i)\r\n", fres);
      break;
    }

    //read the data
    f_gets(buf, sizeof(buf), &fil);

    //close your file
    f_close(&fil);
    printf("\n\rClosing File!\r\n");
  } while( false );

  //We're done, so de-mount the drive
  f_mount(NULL, "", 0);
  printf("\n\rSD Card Unmounted Successfully!\r\n");
}
