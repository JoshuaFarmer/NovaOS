#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "kernel32.h"

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

void setcolor(uint8_t color) {
	colour = color;
}

uint16_t* buffer;

void system(const char* sys);

char* system_user[128];
void kernel_main() {
	init();
	init_pit();
	beep(650, 500);

	identify_ata(0xA0); // master drive

	FAT32BootSector bootsec = read_boot_sector(0xA0);

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
	char cmd[10][128];

	for (int h = 0; h < 10; h++) {
		memset(cmd[h], 0, 128);
	}

	int c = 0;
	int i = 0;
	int j = 0;

	while (sys[i] != '\0') {
		while (sys[i] == ' ')
			i++;

		if (sys[i] == '\0')
			break;

		while (sys[i] != ' ' && sys[i] != '\0') {
			cmd[c][j++] = sys[i++];
		}
		cmd[c][j] = '\0';
		c++;
		j = 0;
	}

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
