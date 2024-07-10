#pragma once
#define FF_FS_READONLY 0

#include <stdarg.h>
#include "typedef.h"

uint8_t system_user[64];
bool_t running = true;

#define setcolor(color) { colour = color }
void system(const uint16_t* sys);

#define PIC1_COMMAND 0x20
#define PIC1_DATA	0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA	0xA1

#include "Kernel/math.h"
#include "Kernel/io.h"
#include "Kernel/terminal.h"
#include "Kernel/sound.h"
#include "Kernel/shell.h"

void init_pic(void) {
	// ICW1: Initialization - starts the PICs
	outb(PIC1_COMMAND, 0x11); // Start initialization sequence for PIC1
	outb(PIC2_COMMAND, 0x11); // Start initialization sequence for PIC2

	// ICW2: Set interrupt vector offsets
	outb(PIC1_DATA, 0x20);   // PIC1 vector offset 0x20-0x27 (IRQ0-IRQ7)
	outb(PIC2_DATA, 0x28);   // PIC2 vector offset 0x28-0x2F (IRQ8-IRQ15)

	// ICW3: Configure cascade connection between PIC1 and PIC2
	outb(PIC1_DATA, 0x04);   // PIC1: IRQ2 is connected to PIC2
	outb(PIC2_DATA, 0x02);   // PIC2: Cascade identity 2

	// OCW1: Enable all IRQs (mask all interrupts)
	outb(PIC1_DATA, 0x0);	// PIC1: Unmask all IRQs
	outb(PIC2_DATA, 0x0);	// PIC2: Unmask all IRQs
}

//#include "ff.h" // Include FatFs header file

#include "Kernel/string.h"
#include "Kernel/malloc.h"
//#include "Kernel/ata.h"
//#include "Kernel/diskio.h"
//#include "ff.h"
//#include "ffconf.h"

//#include "emu.h"
#include "Kernel/video.h"

// Setup VGA buffer. Initialize the VGA frame buffer. (I think - x4exr)