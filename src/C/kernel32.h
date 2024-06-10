#pragma once
#include <stdint.h>

uint8_t system_user[64];
bool running = true;

#define setcolor(color) { colour = color }
void system(const uint16_t* sys);

int abs(int value) {
	return (value < 0) ? -value : value;
}

#include "math.h"
#include "typedef.h"
#include "io.h"
#include "terminal.h"

void syserr(const char* s) {
	printf("%t%s%t\n", VGA_COLOR_LIGHT_RED,s,colour);
	while(1);
}

void init_rtc() {
	cli();		// important that no interrupts happen (perform a CLI)
	outb(0x70, 0x8A);	// select Status Register A, and disable NMI (by setting the 0x80 bit)
	outb(0x71, 0x20);	// write to CMOS/RTC RAM
	sti();		// (perform an STI) and reenable NMI if you wish
}

#include "ff.h" // Include FatFs header file

#include "string.h"
#include "memory.h"
#include "malloc.h"
#include "sound.h"
// #include "ata.h"
#include "diskio.h"
#include "ff.h"
#include "ffconf.h"

#include "emu.h"
#include "video.h"
#include "exception.h"

// Setup VGA buffer. Initialize the VGA frame buffer. (I think - x4exr)
void clsscr() {
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			txtbuff[index] = vga_entry(' ', colour);
		}
	}
}

void init(void) {
	txty = 0;
	txtx = 0;
	colour = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	txtbuff = (uint16_t*) 0xB8000;
	clsscr();
}