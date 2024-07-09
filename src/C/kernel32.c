#include "Kernel/kernel32.h"
#include "GUI/NovaGUI.h"

void init(void) {
	txty = 0;
	txtx = 0;
	colour = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	txtbuff = (uint16_t*) 0xB8000;
	clsscr();
}

void kernel_main() {
	init();

	init_gdt();
	init_idt();

	init_heap();
	init_pit();
	beep(440, 500);

	set_text_mode(1);
	VGA_WIDTH = 90;
	VGA_HEIGHT = 60;

	// idk
	//write_regs(g_320x200x256);
	//memset(((uint8_t*)vgabuff), 0, (320*200));
	//while (1) {
	//	((uint8_t*)vgabuff)[320 * 10 + 10] = 0x1;
	//}

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
