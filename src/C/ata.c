#include "ata.h"

void disable_ata_irq() {
	outb(ATA_PRIMARY_CONTROL_BASE, 0x02);
}

void ata_wait_bsy() {
	while (inb(ATA_PRIMARY_IO + 7) & 0x80);
}

void ata_wait_drq() {
	while (!(inb(ATA_PRIMARY_IO + 7) & 0x08));
}

void ata_select_drive(uint8_t drive) {
	outb(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, 0xA0 | (drive << 4));
	ata_wait_bsy();
}

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
				printf("Master Drive is the current active Drive\n");
				break;
			case 0xB0:
				printf("Slave Drive is the current active Drive\n");
				break;
		}
	}
	else
	{
		switch (drive) {
			case 0xA0:
				printf("Master Drive is NOT the current active Drive\n");
				break;
			case 0xB0:
				printf("Slave Drive is NOT the current active Drive\n");
				break;
			}
	}
}

void ata_read_sector(uint32_t lba, uint8_t *buffer) {
	outb(ATA_PRIMARY_CONTROL_BASE, 0x02);
	outb(ATA_PRIMARY_IO + 6, 0xE0 | ((lba >> 24) & 0x0F));
	outb(ATA_PRIMARY_IO + 1, 0x00);
	outb(ATA_PRIMARY_IO + 2, 1); // Sector count
	outb(ATA_PRIMARY_IO + 3, (uint8_t) lba);
	outb(ATA_PRIMARY_IO + 4, (uint8_t)(lba >> 8));
	outb(ATA_PRIMARY_IO + 5, (uint8_t)(lba >> 16));
	outb(ATA_PRIMARY_IO + 7, ATA_CMD_READ_PIO);

	ata_wait_bsy();
	ata_wait_drq();

	for (int i = 0; i < 256; i++) {
		((uint16_t*)buffer)[i] = inw(ATA_PRIMARY_IO); // Use inw instead of inb
	}
}

void ata_write_sector(uint32_t lba, const byte_t* buffer) {
	outb(ATA_PRIMARY_CONTROL_BASE, 0x02);
	outb(ATA_PRIMARY_IO + 6, 0xE0 | ((lba >> 24) & 0x0F));
	outb(ATA_PRIMARY_IO + 1, 0x00);
	outb(ATA_PRIMARY_IO + 2, 1); // Sector count
	outb(ATA_PRIMARY_IO + 3, (uint8_t) lba);
	outb(ATA_PRIMARY_IO + 4, (uint8_t)(lba >> 8));
	outb(ATA_PRIMARY_IO + 5, (uint8_t)(lba >> 16));
	outb(ATA_PRIMARY_IO + 7, ATA_CMD_WRITE_PIO);

	ata_wait_bsy();

	for (int i = 0; i < 256; i++) {
		outw(ATA_PRIMARY_IO, ((uint16_t*)buffer)[i]); // Use outw instead of outb
	}

	ata_wait_bsy();
}

uint8_t ata_disk_status() {
	uint8_t status = inb(ATA_PRIMARY_IO + 7);
	if (status & STATUS_ERR) {
		printf("Error occurred\n");
	}
	if (status & STATUS_DF) {
		printf("Drive fault\n");
	}
	if (status & STATUS_RDY) {
		printf("Drive is ready\n");
		status = 0;
	}

	return status;
}

uint8_t init_ata() {
	disable_ata_irq();
	ata_select_drive(0);

	return ata_disk_status();
}

void ata_read(byte_t* buff, uint32_t lba, uint32_t count) {
	byte_t* buffer = buff;
	for(size_t i = 0; i < count; ++i) {
		ata_read_sector(lba+i, buffer);
		buffer += 512;
	}
}

void ata_write(const byte_t* buff, uint32_t lba, uint32_t count) {
	const byte_t* buffer = buff;
	for(size_t i = 0; i < count; ++i) {
		ata_write_sector(lba+i, buffer);
		buffer += 512;
	}
}