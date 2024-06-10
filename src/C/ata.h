#pragma once
#include "typedef.h"
#include "terminal.h"
#include "io.h"

#define ATA_PRIMARY_IO_BASE      0x1F0
#define ATA_PRIMARY_CONTROL_BASE 0x3F6
#define ATA_DRIVE_MASTER         0xA0
#define ATA_DRIVE_SLAVE          0xB0

#define ATA_REG_DATA             0x00
#define ATA_REG_ERROR            0x01
#define ATA_REG_FEATURES         0x01
#define ATA_REG_SECCOUNT0        0x02
#define ATA_REG_LBA0             0x03
#define ATA_REG_LBA1             0x04
#define ATA_REG_LBA2             0x05
#define ATA_REG_HDDEVSEL         0x06
#define ATA_REG_COMMAND          0x07
#define ATA_REG_STATUS           0x07
#define ATA_REG_ALTSTATUS        0x0C
#define ATA_REG_CONTROL          0x0C

#define ATA_CMD_READ_PIO         0x20
#define ATA_SR_BSY               0x80
#define ATA_SR_DRDY              0x40
#define ATA_SR_DRQ               0x08

void identify_ata(uint8_t drive) {
	outb(0x1F6, drive);
	outb(0x1F2, 0);
	outb(0x1F3, 0);
	outb(0x1F4, 0);
	outb(0x1F5, 0);
	outb(0x1F7, 0xEC);
	uint8_t tmp = inb(0x1F7);
	if (tmp & STATUS_RDY) {
		switch (drive) {
			case 0xA0:
				printf("%tMaster Drive is the current active Drive\n",VGA_COLOR_LIGHT_GREEN);
				break;
			case 0xB0:
				printf("%t.Slave Drive is the current active Drive\n",VGA_COLOR_LIGHT_GREEN);
				break;
		}
	}
	else
	{
		switch (drive) {
			case 0xA0:
				printf("%tMaster Drive is NOT the current active Drive\n",VGA_COLOR_LIGHT_GREEN);
				break;
			case 0xB0:
				printf("%tSlave Drive is NOT the current active Drive\n",VGA_COLOR_LIGHT_GREEN);
				break;
			}
	}
}

void wait_BSY() { while(inb(0x1F7) & STATUS_BSY); }
void wait_DRQ() { while(!(inb(0x1F7) & STATUS_RDY)); }
void insw(uint16_t port, void* addr, uint32_t count) {
    __asm__ volatile ("rep insw" : "+D"(addr), "+c"(count) : "d"(port) : "memory");
}

void ata_wait_bsy() {
    while (inb(ATA_PRIMARY_IO_BASE + ATA_REG_STATUS) & ATA_SR_BSY);
}

void ata_wait_drq() {
    while (!(inb(ATA_PRIMARY_IO_BASE + ATA_REG_STATUS) & ATA_SR_DRQ));
}

void ata_read_sector(uint32_t lba, uint8_t* buffer) {
    outb(ATA_PRIMARY_CONTROL_BASE, 0x02); // Disable IRQs

    ata_wait_bsy();
    outb(ATA_PRIMARY_IO_BASE + ATA_REG_HDDEVSEL, ATA_DRIVE_MASTER | ((lba >> 24) & 0x0F));
    outb(ATA_PRIMARY_IO_BASE + ATA_REG_SECCOUNT0, 1); // Number of sectors to read
    outb(ATA_PRIMARY_IO_BASE + ATA_REG_LBA0, (uint8_t)(lba & 0xFF));
    outb(ATA_PRIMARY_IO_BASE + ATA_REG_LBA1, (uint8_t)((lba >> 8) & 0xFF));
    outb(ATA_PRIMARY_IO_BASE + ATA_REG_LBA2, (uint8_t)((lba >> 16) & 0xFF));
    outb(ATA_PRIMARY_IO_BASE + ATA_REG_COMMAND, ATA_CMD_READ_PIO);

    ata_wait_bsy();
    ata_wait_drq();

    insw(ATA_PRIMARY_IO_BASE + ATA_REG_DATA, buffer, 256); // Read 256 words (512 bytes)

    outb(ATA_PRIMARY_CONTROL_BASE, 0x00); // Enable IRQs
}
