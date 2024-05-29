#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static inline unsigned char inb(unsigned short port) {
	unsigned char ret;
	asm("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

static inline void outb(unsigned short port, unsigned char value) {
	asm("outb %0, %1" :: "a"(value), "Nd"(port));
}

static inline void outw(uint16_t port, uint16_t value) {
	asm("outw %0, %1" :: "a"(value), "Nd"(port));
}

static inline uint16_t inw(uint16_t port) {
	uint16_t value;
	asm("inw %1, %0" : "=a"(value) : "Nd"(port));
	return value;
}

static inline void outl(uint16_t port, uint32_t value) {
	asm("outl %%eax, %%dx" :: "d" (port), "a" (value));
}

static inline uint32_t inl(uint16_t port) {
	uint32_t ret;
	asm("inl %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
} 