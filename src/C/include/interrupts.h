#pragma once
#include "typedef.h"
#include "pit.h"
#include "GUI/text.h"
#include "GUI/element.h"
#include "GUI/graphics.h"
#include "GUI/video.h"
#include "GUI/window.h"
#include "GUI/taskbar.h"

void window_mngr0();

struct idt_entry {
	uint16_t base_low;	 // Lower 16 bits of the handler address
	uint16_t selector;	 // Kernel segment selector
	uint8_t  always0;	  // This must always be zero
	uint8_t  flags;		// Flags
	uint16_t base_high;	// Upper 16 bits of the handler address
} __attribute__((packed));

struct idt_ptr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

#define IDT_ENTRIES 256
struct idt_entry idt[IDT_ENTRIES];
struct idt_ptr idtp;

void set_idt_entry(int n, uint32_t handler) {
	idt[n].base_low = handler & 0xFFFF;
	idt[n].selector = 0x08; // Kernel code segment
	idt[n].always0 = 0;
	idt[n].flags = 0x8E;	// Present, ring 0, 32-bit interrupt gate
	idt[n].base_high = (handler >> 16) & 0xFFFF;
}

void load_idt() {
	idtp.limit = (sizeof(struct idt_entry) * IDT_ENTRIES) - 1;
	idtp.base = (uint32_t)&idt;
	__asm__ __volatile__("lidt (%0)" : : "r" (&idtp));
}

struct gdt_entry {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t  base_middle;
	uint8_t  access;
	uint8_t  granularity;
	uint8_t  base_high;
} __attribute__((packed));

struct gdt_ptr {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

struct gdt_entry gdt[3];
struct gdt_ptr gdtp;

void set_gdt_entry(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
	gdt[num].base_low = (base & 0xFFFF);
	gdt[num].base_middle = (base >> 16) & 0xFF;
	gdt[num].base_high = (base >> 24) & 0xFF;

	gdt[num].limit_low = (limit & 0xFFFF);
	gdt[num].granularity = (limit >> 16) & 0x0F;
	gdt[num].granularity |= (gran & 0xF0);
	gdt[num].access = access;
}

void init_gdt() {
	gdtp.limit = (sizeof(struct gdt_entry) * 3) - 1;
	gdtp.base = (uint32_t)&gdt;

	set_gdt_entry(0, 0, 0, 0, 0);				// Null segment
	set_gdt_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Kernel code segment
	set_gdt_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Kernel data segment
	//set_gdt_entry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User code segment
	//set_gdt_entry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User data segment

	asm volatile("lgdt (%0)" : : "r" (&gdtp));
	asm volatile("movl $0x10, %%eax; \
			  movl %%eax, %%ds; \
			  movl %%eax, %%es; \
			  movl %%eax, %%fs; \
			  movl %%eax, %%gs; \
			  movl %%eax, %%ss; \
			  ljmp $0x08, $flush; \
			  flush:" : : : "memory");
}

extern void system_interrupt80_handler();
extern void system_interrupt0_handler();
extern void keyboard_interrupt_handler();
extern void default_exception_handler();
extern void mouse_interrupt_handler();

uint32_t _syscall(uint32_t X, uint32_t Y, uint32_t Z, uint32_t W) {
	switch(X) {
		case 0x0: // printf (TXT)
			printf((const char*)Y);
		break;

		case 0x1: // getch (TXT)
			return getch();
		break;

		case 0x2: // putc (TXT)
			putc(Y);
		break;

		case 0x3:
			return (uint32_t)init_window(192, 128, (wchar_t*)Y, (int (*)(void *))Z, (void (*)(void *))W);
		break;
	}

	return 0x0;
}

void system_interrupt0() {
	cli();
	outb(PIC1_COMMAND, 0x20);
	window_mngr0();
	sti();
}

void exception_ok(window_t* whnd) {
	remove_window(whnd, &windows);
}

int exception_wm(window_t* whnd) {
	create_text_element(whnd, L"An exception error", 0, 0);
	create_text_element(whnd, L"has occured.", 0, 12);
	create_button_element(whnd, L"ok", &exception_ok, (whnd->w) - (24 >> 1), (whnd->h >> 1) - 2, 24, 12);
	return 0;
}

int example_wu(window_t* whnd) { return 0; }

void default_exception() {
	printf("EXCEPTION ERROR\n");
	if (windowmngr == true) {
		window_t *errwhnd = init_window(160, 60, L"Error", (int (*)(void*))&exception_wm, (void (*)(void*))&example_wu);
		if(add_window(errwhnd)) draw_text(membuff, L"BRUH", 0, 0, 0);
	}
}

void init_idt() {
	for (int i = 0; i < IDT_ENTRIES; i++) {
		set_idt_entry(i, (uint32_t)default_exception_handler);
	}

	set_idt_entry(0x80, (uint32_t)system_interrupt80_handler);
	set_idt_entry(0x20, (uint32_t)system_interrupt0_handler);
	set_idt_entry(0x21, (uint32_t)keyboard_interrupt_handler); // mostly unused

	load_idt();
	printf("IDT LOADED\n");
}
