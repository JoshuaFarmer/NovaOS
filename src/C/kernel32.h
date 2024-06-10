#pragma once
#include <stdint.h>

uint8_t system_user[64];
bool running = true;

#define setcolor(color) { colour = color }
void system(const uint16_t* sys);

int abs(int value) {
	return (value < 0) ? -value : value;
}

#include "typedef.h"
#include "io.h"
#include "terminal.h"
#include "string.h"
#include "memory.h"
#include "malloc.h"
#include "sound.h"
#include "ata.h"
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