#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "kernel32.h"

#include <stdint.h>

#define setcolor(color) { colour = color }
void system(const uint16_t* sys);

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

uint8_t* system_user;
bool running = true;

void kernel_main() {
	init();
	init_heap();
	init_pit();
	beep(650, 500);

	identify_ata(0xA0); // master drive
	uint16_t* kbdbuf = malloc(128 * sizeof(uint16_t));
	system_user = malloc(32);

	memcpy(system_user, "Default", 128);
	
	size_t heap_size = remaining_heap_space();
	print_int(heap_size,VGA_COLOR_LIGHT_GREEN);
	puts_coloured(" Bytes left in the Heap\n", VGA_COLOR_LIGHT_GREEN);

	while (running) {
		puts_coloured((const char*)system_user, VGA_COLOR_LIGHT_BROWN);
		putc_coloured(':',VGA_COLOR_LIGHT_GREY);
		putc_coloured('/', VGA_COLOR_LIGHT_BLUE);
		puts_coloured("$ ", VGA_COLOR_LIGHT_GREY);
		gets((uint16_t*)kbdbuf, 128);

		system((const uint16_t*)kbdbuf);
	}

	free(kbdbuf);
	free(system_user);
	outw(0x604,0x2000); // qemu only iirc
}

void text_editor();

void system(const uint16_t* sys) {
	char cmd[10][128];

	for (int h = 0; h < 10; h++) {
		memset(cmd[h], 0, 128);
	}

	int c = 0;
	int i = 0;
	int j = 0;

	while (sys[i] != '\0') {
		while (sys[i] == ' ')
			i+=1;

		if (sys[i] == '\0')
			break;

		while (sys[i] != ' ' && sys[i] != '\0') {
			cmd[c][j++] = sys[i++];
		}
		cmd[c][j] = '\0';
		++c;
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

	else if (strcmp(cmd[0], "exit") == 0) {
		running = false;
	}

	else if (strcmp(cmd[0], "edit") == 0) {
		text_editor();
	}

	else if (strcmp(cmd[0], "") == 0) {}
	else {
		puts("COMMAND: "); puts(cmd[0]); putc('\n');
		puts("ARG(S): "); putc('\n');
		for (int k = 1; k < c; ++k) {;
			puts(cmd[k]); putc('\n');
		}
	}
}

void text_editor() {
	clsscr();
	txtx = 0;
	txty = 0;
	update_cursor(txtx,txty);

	uint16_t c = 0;
	bool running = true;

	while (running) {
		c = getch();
		if (c == 0xF001) {
			c = getch();
			if (c == 'c') return;
		} else {
			putc(c);
		}
	}
}