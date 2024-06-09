#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>

#include "kernel32.h"
#include "shell.h"

#include <stdint.h>

void kernel_main() {
	init();
	init_heap();
	init_pit();
	beep(650, 500);

	// ignore for now
	set_text_mode(1);
	VGA_WIDTH = 90;
	VGA_HEIGHT = 60;

	identify_ata(0xA0);
	system_user = malloc(64);
	memcpy(system_user, "default", 128);

	printf("%t#############\n%t#############\n%t#############\n%t#############\n%t#############\n", VGA_COLOR_LIGHT_CYAN,VGA_COLOR_LIGHT_RED,VGA_COLOR_WHITE,VGA_COLOR_LIGHT_RED,VGA_COLOR_LIGHT_CYAN);

	uint16_t* kbdbuf = malloc(128 * sizeof(uint16_t));
	uint8_t* current_dir = malloc(12);
	strcpy((char*)current_dir, "/root/");
	
	size_t heap_size = remaining_heap_space();
	printf("%theap size: %t%d\n", VGA_COLOR_LIGHT_GREEN,VGA_COLOR_WHITE,HEAP_CAP);
	printf("%tremaining heap space: %t%d\n", VGA_COLOR_LIGHT_GREEN,VGA_COLOR_WHITE,heap_size);

	shell(system_user,current_dir,kbdbuf);

	free(kbdbuf);
	free(system_user);
	free(current_dir);
	outw(0x604,0x2000); // qemu only iirc
}
