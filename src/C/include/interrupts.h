#pragma once
#include "typedef.h"
#include "pit.h"

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
	set_gdt_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
	set_gdt_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment

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

void system_interrupt80() {
	draw_text(membuff, L"INTERRUPT 0x80 CALLED", 0, 0, 255);
	debug_print("INT 0x80 CALLED\n");
}

void system_interrupt0() {
	outb(PIC1_COMMAND, 0x20);
}

void default_exception() {
	printf("EXCEPTION ERROR\n");
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
