/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various existing       */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"         /* Obtains integer types */
#include "diskio.h"     /* Declarations of disk functions */

/* Definitions of physical drive number for each drive */
#define DEV_ATA    0   /* Example: Map ATA to physical drive 0 */

/* ATA controller ports and commands */
#define ATA_DATA        0x1F0
#define ATA_ERROR       0x1F1
#define ATA_SECCOUNT    0x1F2
#define ATA_SECTOR      0x1F3
#define ATA_CYL_LO      0x1F4
#define ATA_CYL_HI      0x1F5
#define ATA_DRIVE       0x1F6
#define ATA_COMMAND     0x1F7
#define ATA_STATUS      0x1F7

#define ATA_CMD_READ    0x20
#define ATA_CMD_WRITE   0x30
#define ATA_CMD_IDENT   0xEC

#define ATA_SR_BSY      0x80
#define ATA_SR_DRDY     0x40
#define ATA_SR_DF       0x20
#define ATA_SR_DSC      0x10
#define ATA_SR_DRQ      0x08
#define ATA_SR_CORR     0x04
#define ATA_SR_IDX      0x02
#define ATA_SR_ERR      0x01

/* Function prototypes */
DSTATUS ATA_disk_status(void);
DSTATUS ATA_disk_initialize(void);
DRESULT ATA_disk_read(BYTE* buff, LBA_t sector, UINT count);
DRESULT ATA_disk_write(const BYTE* buff, LBA_t sector, UINT count);

static inline unsigned char inb(unsigned short port) {
	unsigned char ret;
	asm("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

static inline void outb(unsigned short port, unsigned char value) {
	asm("outb %0, %1" :: "a"(value), "Nd"(port));
}

static inline void outw(uint16_t port, uint16_t value) {
	asm("outw %0, %1" :: "a"(value), "Nd"(port));
}

static inline uint16_t inw(uint16_t port) {
	uint16_t value;
	asm("inw %1, %0" : "=a"(value) : "Nd"(port));
	return value;
}

static void ATA_wait(void) {
    while (inb(ATA_STATUS) & ATA_SR_BSY);
}

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
    BYTE pdrv       /* Physical drive number to identify the drive */
)
{
    DSTATUS stat = 0;

    if (pdrv != DEV_ATA) {
        return STA_NOINIT;
    }

    // Check drive status here

    return stat;
}

/*-----------------------------------------------------------------------*/
/* Initialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
    BYTE pdrv               /* Physical drive number to identify the drive */
)
{
    DSTATUS stat = 0;

    if (pdrv != DEV_ATA) {
        return STA_NOINIT;
    }

    // Send IDENTIFY command to ATA drive
    outb(ATA_DRIVE, 0xA0);  // Select master drive
    outb(ATA_SECCOUNT, 0);
    outb(ATA_SECTOR, 0);
    outb(ATA_CYL_LO, 0);
    outb(ATA_CYL_HI, 0);
    outb(ATA_COMMAND, ATA_CMD_IDENT);
    
    ATA_wait();
    
    if (inb(ATA_STATUS) == 0) {
        return STA_NOINIT;
    }

    // Drive initialized successfully

    return stat;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
    BYTE pdrv,      /* Physical drive number to identify the drive */
    BYTE *buff,     /* Data buffer to store read data */
    LBA_t sector,   /* Start sector in LBA */
    UINT count      /* Number of sectors to read */
)
{
    DRESULT res = RES_OK;

    if (pdrv != DEV_ATA) {
        return RES_PARERR;
    }

    while (count--) {
        // Send READ SECTORS command to ATA drive
        outb(ATA_DRIVE, 0xE0 | ((sector >> 24) & 0x0F));
        outb(ATA_SECCOUNT, 1);
        outb(ATA_SECTOR, sector & 0xFF);
        outb(ATA_CYL_LO, (sector >> 8) & 0xFF);
        outb(ATA_CYL_HI, (sector >> 16) & 0xFF);
        outb(ATA_COMMAND, ATA_CMD_READ);

        ATA_wait();

        if (inb(ATA_STATUS) & ATA_SR_ERR) {
            return RES_ERROR;
        }

        // Read data from ATA_DATA port
        for (int i = 0; i < 256; i++) {
            ((unsigned short*)buff)[i] = inw(ATA_DATA);
        }
        buff += 512;
        sector++;
    }

    return res;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
    BYTE pdrv,          /* Physical drive number to identify the drive */
    const BYTE *buff,   /* Data to be written */
    LBA_t sector,       /* Start sector in LBA */
    UINT count          /* Number of sectors to write */
)
{
    DRESULT res = RES_OK;

    if (pdrv != DEV_ATA) {
        return RES_PARERR;
    }

    while (count--) {
        // Send WRITE SECTORS command to ATA drive
        outb(ATA_DRIVE, 0xE0 | ((sector >> 24) & 0x0F));
        outb(ATA_SECCOUNT, 1);
        outb(ATA_SECTOR, sector & 0xFF);
        outb(ATA_CYL_LO, (sector >> 8) & 0xFF);
        outb(ATA_CYL_HI, (sector >> 16) & 0xFF);
        outb(ATA_COMMAND, ATA_CMD_WRITE);

        ATA_wait();

        if (inb(ATA_STATUS) & ATA_SR_ERR) {
            return RES_ERROR;
        }

        // Write data to ATA_DATA port
        for (int i = 0; i < 256; i++) {
            outw(ATA_DATA, ((unsigned short*)buff)[i]);
        }
        buff += 512;
        sector++;
    }

    return res;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
    BYTE pdrv,      /* Physical drive number (0..) */
    BYTE cmd,       /* Control code */
    void *buff      /* Buffer to send/receive control data */
)
{
    DRESULT res = RES_OK;

    if (pdrv != DEV_ATA) {
        return RES_PARERR;
    }

    // Process miscellaneous commands for the ATA drive
    // Example: GET_SECTOR_COUNT, GET_SECTOR_SIZE, etc.

    return res;
}

/* Implementation of ATA functions */

DSTATUS ATA_disk_status(void) {
    // Check ATA drive status and return appropriate DSTATUS
    ATA_wait();
    if (inb(ATA_STATUS) & (ATA_SR_ERR | ATA_SR_DF)) {
        return STA_NOINIT;
    }
    return 0;
}

DSTATUS ATA_disk_initialize(void) {
    // Initialize ATA drive
    outb(ATA_DRIVE, 0xA0);  // Select master drive
    outb(ATA_SECCOUNT, 0);
    outb(ATA_SECTOR, 0);
    outb(ATA_CYL_LO, 0);
    outb(ATA_CYL_HI, 0);
    outb(ATA_COMMAND, ATA_CMD_IDENT);

    ATA_wait();

    if (inb(ATA_STATUS) == 0) {
        return STA_NOINIT;
    }

    // Drive initialized successfully
    return 0;
}

DRESULT ATA_disk_read(BYTE* buff, LBA_t sector, UINT count) {
    // Read sectors from ATA drive
    return disk_read(DEV_ATA, buff, sector, count);
}

DRESULT ATA_disk_write(const BYTE* buff, LBA_t sector, UINT count) {
    // Write sectors to ATA drive
    return disk_write(DEV_ATA, buff, sector, count);
}
