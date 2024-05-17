#pragma once
#include <stdint.h>


#include "typedef.h"

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg);
static inline uint16_t vga_entry(unsigned char uc, uint8_t color);
static inline unsigned char inb(unsigned short port);
static inline void outb(unsigned short port, unsigned char value);
static inline void outw(uint16_t port, uint16_t value);
static inline uint16_t inw(uint16_t port);
static inline void outl(uint16_t port, uint32_t value);
static inline uint32_t inl(uint16_t port);

void* malloc(size_t size);
void init_heap(size_t initial_size);
void free(void* ptr);

void *memset(void *ptr, int value, size_t num);
void* memcpy(void *dest, const void *src, size_t len);
uint32_t memcmp(const void *s1, const void *s2, size_t n);

uint8_t ctoi(const uint8_t c);

char getch();
void gets(char *buffer, const int buffer_size);
void update_cursor(const int x, const int y);
void putch(const char c, const uint8_t colour, const size_t txtx, const size_t txty);
void putc(const char c);
void puth(const uint8_t a);
void putcc(const char c, const uint8_t col);
void putss(const char* data, const size_t size);
void putsc(const char* data, const uint8_t col);
void puts(const char* data);

void identify_ata(uint8_t drive);
uint16_t* LBA28_read_sector(uint8_t drive, uint32_t LBA, uint32_t sector, uint16_t *addr);
void LBA28_write_sector(uint8_t drive, uint32_t LBA, uint32_t sector, uint16_t *buffer);
void decode_raw_disk_info(raw_disk_info dinfo, disk_info * result);
raw_disk_info retrieve_disk_info();
void wait_BSY();
void wait_DRQ();
void init_pit();

void wait_ms(unsigned int milliseconds);
static void playSound(uint32_t nFrequence);
static void shutup();
void beep(uint32_t pitch, uint32_t ms);

char* strpbrk(const char* s, const char* accept);
char* strtok(char* str, const char* delim);
char* strcpy(register char *to, register const char *from);
char* strdup(const char* s);
int strcmp(const char *s1, const char *s2);
size_t strlen(const char* str);

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

uint8_t colour;
uint16_t* txtbuff;

size_t txty=0;
size_t txtx=0;

const unsigned char keyboard_map[256] = {
	0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 'C', 'a', 's',
	'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '#', 'z', 'x', 'c', 'v',
	'b', 'n', 'm', ',', '.', '/', 0xFD, '*', 'A', ' ',
	'C', 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFE, 'n', '7',
	'8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\n',
	'A', 'B', 0xFB, 0xFC
};

const unsigned char keyboard_map_shifted[256] = {
	0, 27, '!', '"', '$', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t',
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 'C', 'A', 'S',
	'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '@', '`', 0, '#', 'Z', 'X', 'C', 'V',
	'B', 'N', 'M', ',', '.', '?', 0xFD, '*', 'A', ' ',
	'C', 0XF1, 0XF2, 0XF3, 0XF4, 0XF5, 0XF6, 0XF7, 0XF8, 0XF9, 0XFA, 0xFE, 0xFE, '7',
	'8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\n',
	'A', 'B', 0XFB, 0XFC
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

// 27 == esc
// 0xf1 - 0xfc are function keys
// 0xfe is numlock
// 0xfd is alt

char getch() {
	char status;
	do {
		status = inb(KEYBOARD_STATUS_PORT); // Read keyboard status port
	} while ((status & 0x01) == 0); // Wait until there is data to read

	unsigned char scancode = inb(KEYBOARD_DATA_PORT); // Read scan code

	static int shift_pressed = 0;

	if (scancode == 0x2A) { // Scan code for Left Shift pressed
		shift_pressed = 1;
		return 0; // Ignore Shift key press itself
	} else if (scancode == 0xAA) { // Scan code for Left Shift released
		shift_pressed = 0;
		return 0; // Ignore Shift key release itself
	}

	if (scancode & 0x80) {
		return 0;
	} else {
		// Check if Shift key is pressed and adjust keyboard map index accordingly
		if (shift_pressed) {
			return keyboard_map_shifted[scancode];
		} else {
			return keyboard_map[scancode];
		}
	}
}

void gets(char *buffer, const int buffer_size) {
	int index = 0;
	char c;

	while (index < buffer_size - 1) { // Ensure space for null-terminator
		c = getch();

		if (c == '\n') { // Check for Enter key
			buffer[index] = '\0'; // Null-terminate the string
			putc('\n');
			return;
		} else if (c == '\b') { // Check for backspace
			if (index > 0) { // Ensure there are characters to delete
				index--;
				putc('\b'); // Move cursor back
				putc(' '); // Erase character
				putc('\b'); // Move cursor back again
			}
		} else if (c >= ' ' && c <= '~') { // Check if printable ASCII character
			buffer[index++] = c; // Store printable character in buffer
			putc(c); // Echo the character to the screen
		}
	}

	// If the loop exits, the buffer is full
	buffer[index] = '\0'; // Null-terminate the string
	putc('\n');
}

void update_cursor(const int x, const int y) {
	uint16_t pos = y * VGA_WIDTH + x;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void putch(const char c, const uint8_t colour, const size_t txtx, const size_t txty) {
	if (c == '\b') return;
	if (c == 0) return;
	if (c == '\n') return;

	// normal
	((char*)videobuff)[(txty*80*2)+(txtx*2)] = c;
	((char*)videobuff)[(txty*80*2)+(txtx*2)+1] = colour;
	outb(0xe9, c);
}

void putc(const char c) {
	if (txty < 0) txty = 0;
	if (txtx < 0) txtx = 0;
	// backspace
	if (c == '\b') {--txtx;
		if (txtx < 0) {
			txtx = 0; --txty;
			while ( ((char*)videobuff)[(txty*80*2) + (txtx*2)] != '\0') {txtx++;}
		}
		((char*)videobuff)[(txty*80*2)+(txtx*2)] = '\0';
		((char*)videobuff)[(txty*80*2)+(txtx*2)+1] = colour;
		outb(0xe9, c);
		update_cursor(txtx, txty);
		return;
	}

	// NULL
	if (c == 0) return;
	if (c == '\n') {txtx = 0; txty++; update_cursor(txtx, txty); return;}

	// normal
	((char*)videobuff)[(txty*80*2)+(txtx*2)] = c;
	((char*)videobuff)[(txty*80*2)+(txtx*2)+1] = colour;
	if (txtx <= 80) {
		txtx++;
	} else {
		txtx = 0; txty++;
	}
	outb(0xe9, c);
	update_cursor(txtx, txty);
}

uint8_t ctoi(const uint8_t c) {
	uint8_t r=c;
	r&=15;
	if (r <= 9) {
		return r + '0';
	} else if (r >= 0xA && r <= 0xF) {
		return r + ('A'-10);
	}
	return 0;
}

void puth(const uint8_t a) {
	char c = ctoi(a>>4);
	putc(c);
	c = ctoi(a&15);
	putc(c);
}

void putcc(const char c, const uint8_t col) {
	uint8_t tmp = colour;
	colour = col;
	putc(c);
	colour = tmp;
}

void putss(const char* data, const size_t size) {
	for (size_t i = 0; i < size; i++)
		putc(data[i]);
}

void putsc(const char* data, const uint8_t col) {
	uint8_t tmp = colour;
	colour = col;
	putss(data, strlen(data));
	colour = tmp;
}

void puts(const char* data) {
	putss(data, strlen(data));
}

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
				putsc("Master Drive is the current active Drive\n", VGA_COLOR_LIGHT_GREEN);
				break;
			case 0xB0:
				putsc("Slave Drive is the current active Drive\n", VGA_COLOR_LIGHT_GREEN);
				break;
		}
	}
	else
	{
		switch(drive){
			case 0xA0:
				putsc("Master Drive is NOT the current active Drive \n", VGA_COLOR_LIGHT_RED);
				break;
			case 0xB0:
				putsc("Slave Drive is NOT the current active Drive \n", VGA_COLOR_LIGHT_RED);
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
	outb(0x1F7, 0x20); // 0x20 = 'Read' Command

	
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
	outb(0x1F6, drive | ((LBA >> 24) & 0xF));		// send drive and bits 24 - 27 of LBA
	outb(0x1F1, 0x00);								// ?
	outb(0x1F2, sector);							// send number of sectors
	outb(0x1F3, (uint8_t) LBA);						// send bits 0-7 of LBA
	outb(0x1F4, (uint8_t) (LBA >> 8));				// 8-15
	outb(0x1F5, (uint8_t) (LBA >> 16)); 			// 16-23
	outb(0x1F7,0x30); 								// 0x30 = 'Write' Command

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

FAT32BootSector read_boot_sector(uint8_t drive) {
	FAT32BootSector boot_sector;
	memset(&boot_sector, 0, sizeof(FAT32BootSector)); // Initialize struct with zeros
	
	LBA28_read_sector(drive, 1, 1, (uint16_t*)&boot_sector); // Corrected pointer cast
	
	return boot_sector;
}

void free_boot_sector(FAT32BootSector* boot_sector) {
	free(boot_sector);
}

// Read the FAT table entry for a given cluster
uint32_t read_fat_entry(uint8_t drive, FAT32BootSector* boot_sector, uint32_t cluster) {
	uint32_t fat_sector = boot_sector->reserved_sectors + (cluster * 4) / boot_sector->bytes_per_sector;
	uint16_t* fat_buffer = (uint16_t*)malloc(SECTOR_SIZE);
	read_sector(drive, fat_sector, fat_buffer);
	uint32_t fat_offset = (cluster * 4) % boot_sector->bytes_per_sector;
	uint32_t fat_entry = *((uint32_t*)(fat_buffer + fat_offset));
	free(fat_buffer);
	return fat_entry;
}

// Write to a FAT table entry
void write_fat_entry(uint8_t drive, FAT32BootSector* boot_sector, uint32_t cluster, uint32_t value) {
	uint32_t fat_sector = boot_sector->reserved_sectors + (cluster * 4) / boot_sector->bytes_per_sector;
	uint16_t* fat_buffer = (uint16_t*)malloc(SECTOR_SIZE);
	read_sector(drive, fat_sector, fat_buffer);
	uint32_t fat_offset = (cluster * 4) % boot_sector->bytes_per_sector;
	*((uint32_t*)(fat_buffer + fat_offset)) = value;
	write_sector(drive, fat_sector, fat_buffer);
	free(fat_buffer);
}

void read_file(uint8_t drive, FAT32BootSector* boot_sector, const char* filepath) {
	uint32_t cluster = boot_sector->root_cluster;
	DirectoryEntry dir_entry;

	// Tokenize the filepath to extract directory names
	char* token = strtok((char*)filepath, "/");
	while (token != NULL) {
		// Traverse the directory tree to find the directory
		while (cluster < 0x0FFFFFF8) {
			uint32_t sector = boot_sector->reserved_sectors + boot_sector->fat_count * boot_sector->sectors_per_fat + (cluster - 2) * boot_sector->sectors_per_cluster;
			uint16_t sector_buffer[SECTOR_SIZE];
			read_sector(drive, sector, sector_buffer);
			for (uint32_t i = 0; i < boot_sector->sectors_per_cluster * boot_sector->bytes_per_sector / sizeof(DirectoryEntry); i++) {
				memcpy(&dir_entry, sector_buffer + i * sizeof(DirectoryEntry), sizeof(DirectoryEntry));
				if (memcmp(token, dir_entry.filename, strlen(token)) == 0) {
					// Directory found, update the cluster and break out of the loop
					cluster = (dir_entry.first_cluster_high << 16) | dir_entry.first_cluster_low;
					break;
				}
			}
			cluster = read_fat_entry(drive, boot_sector, cluster);
		}
		token = strtok(NULL, "/");
	}

	// At this point, cluster should point to the directory containing the target file
	// Now, search for the file within this directory
	while (cluster < 0x0FFFFFF8) {
		uint32_t sector = boot_sector->reserved_sectors + boot_sector->fat_count * boot_sector->sectors_per_fat + (cluster - 2) * boot_sector->sectors_per_cluster;
		uint16_t sector_buffer[SECTOR_SIZE];
		read_sector(drive, sector, sector_buffer);
		for (uint32_t i = 0; i < boot_sector->sectors_per_cluster * boot_sector->bytes_per_sector / sizeof(DirectoryEntry); i++) {
			memcpy(&dir_entry, sector_buffer + i * sizeof(DirectoryEntry), sizeof(DirectoryEntry));
			if (memcmp(filepath, dir_entry.filename, strlen(filepath)) == 0) {
				// File found
				uint32_t file_cluster = (dir_entry.first_cluster_high << 16) | dir_entry.first_cluster_low;
				uint8_t* file_data = (uint8_t*)malloc(dir_entry.file_size);
				if (file_data == NULL) {
					putsc("Memory allocation error\n", VGA_COLOR_LIGHT_RED);
					return;
				}
				memset(file_data, 0, dir_entry.file_size);
				uint8_t* file_data_start = file_data; // Store the base address
				while (file_cluster < 0x0FFFFFF8) {
					uint32_t sector = boot_sector->reserved_sectors + boot_sector->fat_count * boot_sector->sectors_per_fat +
									  (file_cluster - 2) * boot_sector->sectors_per_cluster;
					read_sector(drive, sector, (uint16_t*)file_data);
					file_data += boot_sector->sectors_per_cluster * boot_sector->bytes_per_sector;
					file_cluster = read_fat_entry(drive, boot_sector, file_cluster);
				}
				// Print or process the file data as needed
				// Here, we'll just print the first few bytes
				for (uint32_t j = 0; j < 100 && j < dir_entry.file_size; j++) {
					putc(file_data_start[j]);
				}
				free(file_data_start); // Free the original pointer
				return;
			}
		}
		cluster = read_fat_entry(drive, boot_sector, cluster);
	}
	putsc("File not found: ", VGA_COLOR_LIGHT_RED);
	puts(filepath);
	putc('\n');
}

void list_files(uint8_t drive, FAT32BootSector* boot_sector) {
	uint32_t cluster = boot_sector->root_cluster;
	puth(cluster>>24);
	puth(cluster>>16);
	puth(cluster>>8);
	puth(cluster);
	DirectoryEntry dir_entry;
	puts("\nlisting\n");
	// Traverse the directory tree to find the directory
	while (cluster < 0x0FFFFFF8) {
		uint32_t sector = boot_sector->reserved_sectors + boot_sector->fat_count * boot_sector->sectors_per_fat + (cluster - 2) * boot_sector->sectors_per_cluster;
		uint16_t sector_buffer[SECTOR_SIZE];
		read_sector(drive, sector, sector_buffer);
		for (uint32_t i = 0; i < boot_sector->sectors_per_cluster * boot_sector->bytes_per_sector / sizeof(DirectoryEntry); i++) {
			memcpy(&dir_entry, sector_buffer + i * sizeof(DirectoryEntry), sizeof(DirectoryEntry));
			putsc(dir_entry.filename, VGA_COLOR_LIGHT_MAGENTA);
		}
		cluster = read_fat_entry(drive, boot_sector, cluster);
	}
}

#include "io.h"
#include "memory.h"
#include "badmalloc.h"
#include "string.h"
#include "sound.h"