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

// 0xf1 - 0xfc are function keys
// 0xfe is numlock
// 0xfd is alt

char getch() {
	char status;
	do {
		status = inb(KEYBOARD_STATUS_PORT);
	} while ((status & 0x01) == 0);

	unsigned char scancode = inb(KEYBOARD_DATA_PORT);

	static int shift_pressed = 0;

	if (scancode == 0x2A) {
		shift_pressed = 1;
		return 0;
	} else if (scancode == 0xAA) {
		shift_pressed = 0;
		return 0;
	}

	return (scancode & 0x80) ? 0 : (shift_pressed ? keyboard_map_shifted[scancode] : keyboard_map[scancode]);
}

void gets(char *buffer, const int buffer_size) {
	int index = 0;
	char c;

	while (index < buffer_size - 1) {
		c = getch();

		if (c == '\n') {
			buffer[index] = '\0';
			putc('\n');
			return;
		} else if (c == '\b') {
			if (index > 0) {
				index--;
				putc('\b');
				putc(' ');
				putc('\b');
			}
		} else if (c >= ' ' && c <= '~') {
			buffer[index++] = c;
			putc(c);
		}
	}

	buffer[index] = '\0';
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

	((char*)videobuff)[(txty*80*2)+(txtx*2)] = c;
	((char*)videobuff)[(txty*80*2)+(txtx*2)+1] = colour;
	outb(0xe9, c);
}

void putc(const char c) {
	if (txty < 0) txty = 0;
	if (txtx < 0) txtx = 0;

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

	if (c == 0) return;
	if (c == '\n') {txtx = 0; txty++; update_cursor(txtx, txty); return;}

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

#include "io.h"
#include "memory.h"
#include "badmalloc.h"
#include "string.h"
#include "sound.h"