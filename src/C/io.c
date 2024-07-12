#include "Kernel/io.h"

unsigned char inb(unsigned short port) {
	unsigned char ret;
	asm("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

void outb(unsigned short port, unsigned char value) {
	asm("outb %0, %1" :: "a"(value), "Nd"(port));
}

unsigned short inw(unsigned short port) {
	unsigned short value;
	asm("inw %1, %0" : "=a"(value) : "Nd"(port));
	return value;
}

void outw(unsigned short port, unsigned short value) {
	asm("outw %0, %1" :: "a"(value), "Nd"(port));
}

unsigned int inl(unsigned short port) {
	unsigned int ret;
	asm("inl %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
} 

void outl(unsigned short port, unsigned int value) {
	asm("outl %%eax, %%dx" :: "d" (port), "a" (value));
}

void insw(unsigned short port, void* addr, unsigned int count) {
	__asm__ volatile ("rep insw" : "+D"(addr), "+c"(count) : "d"(port) : "memory");
}