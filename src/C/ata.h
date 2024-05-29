#pragma once
#include "typedef.h"
#include "terminal.h"
#include "io.h"

void identify_ata(uint8_t drive){
	outb(0x1F6, drive);
	outb(0x1F2, 0);
	outb(0x1F3, 0);
	outb(0x1F4, 0);
	outb(0x1F5, 0);
	outb(0x1F7, 0xEC);
	uint8_t tmp = inb(0x1F7);
	if(tmp & STATUS_RDY){
		switch(drive){
			case 0xA0:
				puts_coloured("Master Drive is the current active Drive\n", VGA_COLOR_LIGHT_GREEN);
				break;
			case 0xB0:
				puts_coloured("Slave Drive is the current active Drive\n", VGA_COLOR_LIGHT_GREEN);
				break;
		}
	}
	else
	{
		switch(drive){
			case 0xA0:
				puts_coloured("Master Drive is NOT the current active Drive \n", VGA_COLOR_LIGHT_RED);
				break;
			case 0xB0:
				puts_coloured("Slave Drive is NOT the current active Drive \n", VGA_COLOR_LIGHT_RED);
				break;
			}
	}
}

void wait_BSY() { while(inb(0x1F7) & STATUS_BSY); }
void wait_DRQ() { while(!(inb(0x1F7) & STATUS_RDY)); }

uint16_t* LBA28_read_sector(uint8_t drive, uint32_t LBA, uint32_t sector, uint16_t *addr) {
	LBA &= 0x0FFFFFFF;
	
	wait_BSY();
	outb(0x1F6, drive | ((LBA >> 24) & 0xF));
	outb(0x1F1, 0x00);
	outb(0x1F2, sector);
	outb(0x1F3, (uint8_t) LBA);
	outb(0x1F4, (uint8_t)(LBA >> 8));
	outb(0x1F5, (uint8_t)(LBA >> 16)); 
	outb(0x1F7, 0x20);

	uint16_t *tmp = addr;
	
	for (uint32_t j = 0; j < sector; j ++){
		wait_BSY();
		wait_DRQ();
		for(uint32_t i = 0; i < 256; i++){
			tmp[i] = inw(0x1F0);
		}

		tmp += 256;
	}
	return addr;
}

void LBA28_write_sector(uint8_t drive, uint32_t LBA, uint32_t sector, uint16_t *buffer){
	LBA &= 0x0FFFFFFF;
	
	wait_BSY();
	outb(0x1F6, drive | ((LBA >> 24) & 0xF));
	outb(0x1F1, 0x00);
	outb(0x1F2, sector);
	outb(0x1F3, (uint8_t) LBA);
	outb(0x1F4, (uint8_t) (LBA >> 8));
	outb(0x1F5, (uint8_t) (LBA >> 16));
	outb(0x1F7,0x30);

	uint32_t *tmp = (uint32_t*)buffer;
	
	for (uint32_t j = 0; j < sector; j++){
		wait_BSY();
		wait_DRQ();

		for(uint32_t i = 0; i < 256; i++){
			outl(0x1F0, tmp[i]);
		}

		outb(0x1F7, 0xE7);
		
		tmp += 256;
	}
}

raw_disk_info retrieve_disk_info(){
	raw_disk_info* dinfo = (raw_disk_info*)0x5200;
	return *dinfo;
}

void decode_raw_disk_info(raw_disk_info dinfo, disk_info * result){
	result -> drivetype = dinfo.bl;
	result -> sectors = dinfo.cl & 0b00111111;
	result -> cylinders = ((dinfo.cl & 0b11000000) << 2) | dinfo.ch;
	result -> heads = dinfo.dh;
	result -> drives = dinfo.dl;
}
