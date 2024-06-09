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
	// set_text_mode(1);
	// VGA_WIDTH = 90;
	// VGA_HEIGHT = 60;
	void* vd = malloc(1024);

	identify_ata(0xA0);
	system_user = malloc(64);
	memcpy(system_user, "default", 128);

	puts_coloured("#############\n", VGA_COLOR_LIGHT_CYAN);
	puts_coloured("#############\n", VGA_COLOR_LIGHT_RED);
	puts_coloured("#############\n", VGA_COLOR_WHITE);
	puts_coloured("#############\n", VGA_COLOR_LIGHT_RED);
	puts_coloured("#############\n", VGA_COLOR_LIGHT_CYAN);
	
	fat_BS_t* bs = malloc(512);
	read_boot_sector(0xA0, bs);
	printf("%T10.bytes_per_sector: %T14.%d\n", bs->bytes_per_sector);
    printf("%T10.sectors_per_cluster: %T14.%d\n", bs->sectors_per_cluster);
    printf("%T10.reserved_sector_count: %T14.%d\n", bs->reserved_sector_count);
    printf("%T10.table_count: %T14.%d\n", bs->table_count);
    printf("%T10.total_sectors_16: %T14.%d\n", bs->total_sectors_16);
    printf("%T10.total_sectors_32: %T14.%d\n", bs->total_sectors_32);
    printf("%T10.table_size_16: %T14.%d\n", bs->table_size_16);

	uint16_t* kbdbuf = malloc(128 * sizeof(uint16_t));

	uint8_t* current_dir = malloc(12);
	strcpy((char*)current_dir, "/root/");

	size_t heap_size = remaining_heap_space();
	printf("%T10.heap size:%T14. %d\n", HEAP_CAP);
	printf("%T10.remaining heap space:%T14. %d\n", heap_size);
	
	shell(system_user,current_dir,kbdbuf);

	free(kbdbuf);
	free(system_user);
	free(current_dir);
	outw(0x604,0x2000); // qemu only iirc
}
