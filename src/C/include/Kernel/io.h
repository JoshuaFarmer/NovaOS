#pragma once

unsigned char inb(unsigned short port);
void outb(unsigned short port, unsigned char value);
unsigned short inw(unsigned short port);
void outw(unsigned short port, unsigned short value);
unsigned int inl(unsigned short port);
void outl(unsigned short port, unsigned int value);
void insw(unsigned short port, void* addr, unsigned int count);