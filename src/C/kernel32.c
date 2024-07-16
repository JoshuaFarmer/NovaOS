#include "Kernel/kernel32.h"
#include "GUI/NovaGUI.h"

void init(void) {
	txty = 0;
	txtx = 0;
	colour = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	txtbuff = (uint16_t*) 0xB8000;
	clsscr();
}

void testFatFS() {
	FATFS fs;	 // File system object
	FIL fil;	  // File object
	FRESULT fr;   // FatFS return code
	// BYTE work[FF_MAX_SS]; // Work area (larger is better for some functions)

	// Initialize the disk
	fr = disk_initialize(0);
	if (fr != RES_OK) {
		printf("%tFatFS Disk Error: %d\n", VGA_COLOR_LIGHT_RED,fr);
		return;
	}

	// Mount the filesystem
	fr = f_mount(&fs, "", 1);
	if (fr != FR_OK) {
		printf("%tFatFS Mount Error: %d\n", VGA_COLOR_LIGHT_RED,fr);
		return;
	}

	// Open a file for writing (create if it doesn't exist)
	fr = f_open(&fil, "test.txt", FA_WRITE | FA_CREATE_ALWAYS);
	if (fr != FR_OK) {
		printf("%tFatFS Open(W) Error: %d\n", VGA_COLOR_LIGHT_RED,fr);
		f_mount(NULL, "", 0);  // Unmount the filesystem
		return;
	}

	// Write data to the file
	const char *text = "FatFS test successful!";
	UINT bytesWritten;
	fr = f_write(&fil, text, strlen(text), &bytesWritten);
	if (fr != FR_OK || bytesWritten < strlen(text)) {
		printf("%tFatFS Write Error: %d\n", VGA_COLOR_LIGHT_RED,fr);
		f_close(&fil);
		f_mount(NULL, "", 0);  // Unmount the filesystem
		return;
	}

	// Close the file
	f_close(&fil);

	// Open the file for reading
	fr = f_open(&fil, "test.txt", FA_READ);
	if (fr != FR_OK) {
		printf("%tFatFS Open(R) Error: %d\n", VGA_COLOR_LIGHT_RED,fr);
		f_mount(NULL, "", 0);  // Unmount the filesystem
		return;
	}

	// Read data from the file
	char buffer[64];
	UINT bytesRead;
	fr = f_read(&fil, buffer, sizeof(buffer) - 1, &bytesRead);
	if (fr != FR_OK) {
		printf("%tFatFS Read Error: %d\n", VGA_COLOR_LIGHT_RED,fr);

		f_close(&fil);
		f_mount(NULL, "", 0);  // Unmount the filesystem
		return;
	}

	// Null-terminate the buffer and print the read data
	buffer[bytesRead] = '\0';
	printf("Read from file: %s\n", buffer);

	// Close the file
	f_close(&fil);

	// Unmount the filesystem
	f_mount(NULL, "", 0);
}

void kernel_main() {
	init();

	init_gdt();
	init_idt();
	init_pic();

	init_heap();
	init_pit(12);

	beep(440, 500);

	set_text_mode(1);
	VGA_WIDTH = 90;
	VGA_HEIGHT = 60;

	disable_ata_irq();
	identify_ata(0xA0);
	ata_disk_status();

	testFatFS();

	memcpy(system_user, "default", 32);

	// printf("%t#############\n%t#############\n%t#############\n%t#############\n%t#############\n", VGA_COLOR_LIGHT_CYAN,VGA_COLOR_LIGHT_RED,VGA_COLOR_WHITE,VGA_COLOR_LIGHT_RED,VGA_COLOR_LIGHT_CYAN);
	
	uint16_t* kbdbuf[512];
	uint8_t* current_dir[32];
	strcpy((char*)current_dir, "/");

	size_t heap_size = remaining_heap_space();
	printf("%theap size: %t%d\n", VGA_COLOR_LIGHT_GREEN,VGA_COLOR_WHITE,HEAP_CAP);
	printf("%tremaining heap space: %t%d\n", VGA_COLOR_LIGHT_GREEN,VGA_COLOR_WHITE,heap_size);
	
	txtshell((uint8_t*)system_user,(uint8_t*)current_dir,(uint16_t*)kbdbuf);

	outw(0x604,0x2000); // qemu only iirc
}
