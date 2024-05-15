#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

#include "kernel32.h"
#include "video.h"

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
	//init_graph_vga((unsigned)320, (unsigned)270, 0);
	txty = 0;
	txtx = 0;
	colour = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	txtbuff = (uint16_t*) 0xB8000;
	clsscr();
}

void setcolor(uint8_t color) {
	colour = color;
}

struct multiboot_info {
	uint32_t flags;
	uint32_t mem_lower;
	uint32_t mem_upper;
	// Other fields omitted for brevity
};

uint8_t hchar(uint8_t c) {
	c&=15;
	if (c <= 9) {
		return c + '0';
	} else if (c >= 0xA && c <= 0xF) {
		return c + ('A'-10);
	}
	return 0;
}

void puth(uint8_t a) {
	char c = hchar(a>>4);
	putc(c);
	c = hchar(a&15);
	putc(c);
}

uint16_t* buffer;

void system(const char* sys);

char* system_user[128];
void kernel_main(/*struct multiboot_info* mb_info*/)	{
	init();
	//init_graph_vga(400,256,1);
	init_pit();
	beep(650, 700);

	//uint32_t total_memory=0;
	//if (mb_info->flags & 0x1)
	//	total_memory = (mb_info->mem_lower + mb_info->mem_upper) * 1024;
	
	//raw_disk_info disk = retrieve_disk_info();
	identify_ata(0xA0); // main

	FAT32BootSector* bootsec = read_boot_sector(0xA0);

	read_file(0xA0, bootsec, "a.txt");

	char* kbdbuf[128];
	memcpy(system_user, "Default", 128);

	for (;;) {
		putsc((const char*)system_user, VGA_COLOR_LIGHT_BROWN);
		putcc(':',VGA_COLOR_LIGHT_GREY);
		putcc('/', VGA_COLOR_LIGHT_BLUE);
		putsc("$ ", VGA_COLOR_LIGHT_GREY);
		gets((char*)kbdbuf, 128);

		system((const char*)kbdbuf);
	}
}

void system(const char* sys) {
	char cmd[10][128]; // Assuming maximum 10 words in the command and each word length is at most 127

	for (int h = 0; h < 10; h++) {
		memset(cmd[h], 0, 128);
	}

	int c = 0; // Counter for command words
	int i = 0; // Index within sys string
	int j = 0; // Index within a particular command word

	// Iterate through the input string
	while (sys[i] != '\0') {
		// Skip leading spaces
		while (sys[i] == ' ')
			i++;

		// If end of string, break
		if (sys[i] == '\0')
			break;

		// Copy characters until a space or end of string is encountered
		while (sys[i] != ' ' && sys[i] != '\0') {
			cmd[c][j++] = sys[i++];
		}
		cmd[c][j] = '\0'; // Null-terminate the command word
		c++; // Move to the next command word
		j = 0; // Reset the index for the next word
	}

	// now we have the basic commands
	if (strcmp(cmd[0], "user") == 0) {
		if (c > 1) {
			for (int k = 0; k < c; ++k) {
				if (strcmp(cmd[k], "set") == 0 && k+1 != c) {
					memcpy(system_user, cmd[k+1], 128);
					break;
				}
			}
		}
	}

	else if (strcmp(cmd[0], "clear") == 0) {
		clsscr();
		txtx = 0;
		txty = 0;
	}
	
	else if (strcmp(cmd[0], "") == 0) {}
	else {
		puts("COMMAND: "); puts(cmd[0]); putc('\n');
		puts("ARG(S): "); putc('\n');
		for (int k = 1; k < c; ++k) {
			puts(cmd[k]); putc('\n');
		}
	}
}
