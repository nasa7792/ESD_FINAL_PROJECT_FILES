/* ---------------------------------------------------------------------------------
 * Abhirath Koushik and EmbeTronicX (https://embetronicx.com/tutorials/microcontrollers/stm32/stm32-sd-card-interfacing-with-example/)
 * ECEN 5613 - Fall 2024 - Prof. McClure
 * University of Colorado Boulder
 * Revised 12/14/24
 *  --------------------------------------------------------------------------------
 * This file contains function declarations related to SD Card Disk Operations and Functions.
   ---------------------------------------------------------------------------------*/

#ifndef __FATFS_SD_H
#define __FATFS_SD_H

/* -------------------------------------------------- */
//          INCLUDES & DEFINES
/* -------------------------------------------------- */

/* Defining the SPI Handler and SPI Chip Select Pins */
#define HSPI_SDCARD     &hspi2
#define SD_CS_PORT      GPIOB
#define SD_CS_PIN     GPIO_PIN_12

/* Definitions for MMC/SDC command. Defined by EmbeTronicX */
#define CMD0     (0x40+0)       /* GO_IDLE_STATE */
#define CMD1     (0x40+1)       /* SEND_OP_COND */
#define CMD8     (0x40+8)       /* SEND_IF_COND */
#define CMD9     (0x40+9)       /* SEND_CSD */
#define CMD10    (0x40+10)      /* SEND_CID */
#define CMD12    (0x40+12)      /* STOP_TRANSMISSION */
#define CMD16    (0x40+16)      /* SET_BLOCKLEN */
#define CMD17    (0x40+17)      /* READ_SINGLE_BLOCK */
#define CMD18    (0x40+18)      /* READ_MULTIPLE_BLOCK */
#define CMD23    (0x40+23)      /* SET_BLOCK_COUNT */
#define CMD24    (0x40+24)      /* WRITE_BLOCK */
#define CMD25    (0x40+25)      /* WRITE_MULTIPLE_BLOCK */
#define CMD41    (0x40+41)      /* SEND_OP_COND (ACMD) */
#define CMD55    (0x40+55)      /* APP_CMD */
#define CMD58    (0x40+58)      /* READ_OCR */

/* MMC card type flags (MMC_GET_TYPE). Defined by EmbeTronicX */
#define CT_MMC    0x01    /* MMC ver 3 */
#define CT_SD1    0x02    /* SD ver 1 */
#define CT_SD2    0x04    /* SD ver 2 */
#define CT_SDC    0x06    /* SD */
#define CT_BLOCK  0x08    /* Block addressing */

/* -------------------------------------------------- */
//          GLOBALS
/* -------------------------------------------------- */
#define SPI_TIMEOUT 100
/* SPI HAL Handler */
extern SPI_HandleTypeDef  hspi2;

/* -------------------------------------------------- */
//          FUNCTION DEFINITIONS
/* -------------------------------------------------- */

/*
 * Function to initialize SD Card Disk through SPI.
 *
 * Parameters:
 * 	pdrv : Physical drive number to identify the drive
 *
 * Returns:
 * 	Disk Status (based on Disk Status Bits (DSTATUS) in diskio.h)
 */
DSTATUS SD_disk_initialize (BYTE pdrv);

/*
 * Function to return the SD Card Drive Status directly.
 *
 * Parameters:
 * 	pdrv : Physical drive number to identify the drive
 *
 * Returns:
 * 	Disk Status (based on Disk Status Bits (DSTATUS) in diskio.h)
 */
DSTATUS SD_disk_status (BYTE pdrv);

/*
 * Function to perform a Read operation from the SD Card Drive.
 *
 * Parameters:
 * 	pdrv : Physical drive number to identify the drive
 * 	buff : Pointer to buffer where read data is stored
 * 	sector : Logical sector address to read from
 *  count  : Number of sectors to read.
 *
 * Returns:
 * 	Disk Operation Flags (based on enumeration DRESULT in diskio.h)
 */
DRESULT SD_disk_read (BYTE pdrv, BYTE* buff, DWORD sector, UINT count);

/*
 * Function to perform a Write operation to the SD Card Drive.
 *
 * Parameters:
 * 	pdrv : Physical drive number to identify the drive
 * 	buff : Pointer to buffer where read data is stored
 * 	sector : Logical sector address to read from
 *  count  : Number of sectors to read.
 *
 * Returns:
 * 	Disk Operation Flags (based on enumeration DRESULT in diskio.h)
 */
DRESULT SD_disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count);

/*
 * Function to perform a Write operation to the SD Card Drive.
 *
 * Parameters:
 * 	pdrv : Physical drive number to identify the drive
 * 	cmd : Control command specifying the operation to perform
 * 	buff : Pointer to a buffer used to send or receive data related to the control operation
 *
 * Returns:
 * 	Disk Operation Flags (based on enumeration DRESULT in diskio.h)
 */
DRESULT SD_disk_ioctl (BYTE pdrv, BYTE cmd, void* buff);

#endif  /* __FATFS_SD_H */
