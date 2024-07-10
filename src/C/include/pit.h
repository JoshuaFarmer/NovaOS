#pragma once
#include "typedef.h"

#define PIT_FREQUENCY 1193182
#define PIT_CHANNEL_0 0x40
#define PIT_COMMAND   0x43
#define PIC1_COMMAND  0x20
#define PIC1_DATA	 0x21
#define PIC2_COMMAND  0xA0
#define PIC2_DATA	 0xA1

#define INTERRUPT_GATE 0x8E

void init_pit(uint32_t frequency) {
	uint32_t divisor = PIT_FREQUENCY / frequency;

	outb(PIT_COMMAND, 0x36);  // Command byte: Channel 0, Access mode: lobyte/hibyte, Mode: 3, Binary mode
	outb(PIT_CHANNEL_0, (uint8_t)(divisor & 0xFF));	   // Low byte of divisor
	outb(PIT_CHANNEL_0, (uint8_t)((divisor >> 8) & 0xFF));// High byte of divisor
}