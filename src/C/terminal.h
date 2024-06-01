#pragma once
#include "string.h"
void putc(const uint16_t c);
void scroll_terminal();

static size_t VGA_WIDTH = 80;
static size_t VGA_HEIGHT = 25;

uint8_t colour;
uint16_t* txtbuff;

size_t txty=0;
size_t txtx=0;
size_t tabw = 4;
bool numlock=false;

enum {
	KEY_NULL = 0x0,
	KEY_CTRL = 0xF001,
	KEY_ALT,
	KEY_TAB,
	KEY_NUML,
	KEY_ESC,
	KEY_INS,
	KEY_HOM,
	KEY_PGU,
	KEY_DEL,
	KEY_END,
	KEY_PGD,
	KEY_UP,
	KEY_DN,
	KEY_LE,
	KEY_RI,
	KEY_SCRL, // scroll lock

	KEY_F1 = 0xF010,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
};

const uint16_t keyboard_map[256] = {
	KEY_NULL, KEY_ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', KEY_TAB,
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', KEY_CTRL, 'a', 's',
	'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', KEY_NULL, '#', 'z', 'x', 'c', 'v',
	'b', 'n', 'm', ',', '.', '/', KEY_ALT, '*', KEY_ALT, ' ',
	KEY_CTRL, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_NUML, KEY_SCRL, KEY_HOM,
	KEY_UP, KEY_PGU, '-', KEY_LE, '5', KEY_RI, '+', KEY_END, KEY_DN, KEY_PGD, KEY_INS, KEY_DEL, '\n',
	KEY_ALT, '\\', KEY_F11, KEY_F12
};

const uint16_t keyboard_map_numlock[256] = {
	KEY_NULL, KEY_ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', KEY_TAB,
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', KEY_CTRL, 'a', 's',
	'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', KEY_NULL, '#', 'z', 'x', 'c', 'v',
	'b', 'n', 'm', ',', '.', '/', KEY_ALT, '*', KEY_ALT, ' ',
	KEY_CTRL, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_NUML, KEY_SCRL, '7',
	'8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\n',
	KEY_ALT, '\\', KEY_F11, KEY_F12
};

const uint16_t keyboard_map_numlock_shifted[256] = {
	KEY_NULL, KEY_ESC, '!', '"', '\\', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', KEY_TAB,
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', KEY_CTRL, 'A', 'S',
	'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '@', '`', KEY_NULL, '#', 'Z', 'X', 'C', 'V',
	'B', 'N', 'M', '<', '>', '?', KEY_ALT, '*', KEY_ALT, ' ',
	KEY_CTRL, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_NUML, KEY_SCRL, '7',
	'8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\n',
	KEY_ALT, '\\', KEY_F11, KEY_F12
};

const uint16_t keyboard_map_shifted[256] = {
	KEY_NULL, KEY_ESC, '!', '"', '\\', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', KEY_TAB,
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', KEY_CTRL, 'A', 'S',
	'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '@', '`', KEY_NULL, '#', 'Z', 'X', 'C', 'V',
	'B', 'N', 'M', '<', '>', '?', KEY_ALT, '*', KEY_ALT, ' ',
	KEY_CTRL, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_NUML, '/', '7',
	'8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\n',
	KEY_ALT, '\\', KEY_F11, KEY_F12
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

uint16_t getch() {
	char status;
	do {
		status = inb(KEYBOARD_STATUS_PORT);
	} while ((status & 0x01) == 0);

	unsigned char scancode = inb(KEYBOARD_DATA_PORT);

	static int shift_pressed = 0;

	if (scancode == 0x2A) {
		shift_pressed = 1;
		return 0;
	} else if (scancode == 0xAA) {
		shift_pressed = 0;
		return 0;
	} else if (keyboard_map[scancode] == KEY_NUML) numlock = !numlock;
	
	return (scancode & 0x80) ? 0 : (numlock ? (shift_pressed ? keyboard_map_numlock_shifted[scancode] : keyboard_map_numlock[scancode]): (shift_pressed ? keyboard_map_shifted[scancode] : keyboard_map[scancode]));
}

void gets(uint16_t *buffer, const size_t buffer_size) {
	size_t index = 0;
	uint16_t c;

	while (index < buffer_size - 1) {
		c = getch();

		if (c == '\n') {
			buffer[index] = '\0';
			putc('\n');
			return;
		} else if (c == '\b') {
			if (index > 0) {
				index--;
				putc('\b');
				putc(' ');
				putc('\b');
			}
		} else if (c >= ' ' && c <= '~') {
			buffer[index++] = c;
			putc(c);
		}
	}

	buffer[index] = '\0';
	putc('\n');
}

void update_cursor(const int x, const int y) {
	uint16_t pos = y * VGA_WIDTH + x;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

void putc_at(const uint16_t c, const uint8_t colour, size_t txtx, size_t txty) {
	if (txty < 0) txty = 0;
	if (txtx < 0) txtx = 0;

	if (c == '\b') {--txtx;
		if (txtx < 0) {
			txtx = 0; --txty;
			while ( ((char*)videobuff)[(txty*VGA_WIDTH*2) + (txtx*2)] != '\0') {txtx++;};
		}
		((char*)videobuff)[(txty*VGA_WIDTH*2)+(txtx*2)] = '\0';
		((char*)videobuff)[(txty*VGA_WIDTH*2)+(txtx*2)+1] = colour;
		outb(0xe9, c);
		update_cursor(txtx, txty);
		return;
	}

	if (c == 0) return;
	if (c == '\n') {txtx = 0; txty++; update_cursor(txtx, txty); return;}
	if (c == KEY_TAB) {for (size_t i = 0; i < tabw; ++i) putc_at(' ', colour, txtx, txty); return;}

	((char*)videobuff)[(txty*VGA_WIDTH*2)+(txtx*2)] = c;
	((char*)videobuff)[(txty*VGA_WIDTH*2)+(txtx*2)+1] = colour;
	outb(0xe9, c);
}

void putc(const uint16_t c) {
	if (txty < 0) txty = 0;
	if (txtx < 0) txtx = 0;
	if (txty >= VGA_HEIGHT) scroll_terminal();

	if (c == '\b') {--txtx;
		if (txtx < 0) {
			txtx = 0; --txty;
			while ( ((char*)videobuff)[(txty*VGA_WIDTH*2) + (txtx*2)] != '\0') {txtx++;};
		}
		((char*)videobuff)[(txty*VGA_WIDTH*2)+(txtx*2)] = '\0';
		((char*)videobuff)[(txty*VGA_WIDTH*2)+(txtx*2)+1] = colour;
		outb(0xe9, c);
		update_cursor(txtx, txty);
		return;
	}

	if (c == 0) return;
	if (c == '\n') {txtx = 0; txty++; update_cursor(txtx, txty); return;}
	if (c == KEY_TAB) {for (size_t i = 0; i < tabw; ++i) putc(' '); return;}

	((char*)videobuff)[(txty*VGA_WIDTH*2)+(txtx*2)] = c;
	((char*)videobuff)[(txty*VGA_WIDTH*2)+(txtx*2)+1] = colour;

	if (txtx <= VGA_WIDTH) {
		txtx++;
	} else {
		txtx = 0; txty++;
	}
	outb(0xe9, c);
	update_cursor(txtx, txty);
}

void putc_coloured(const char c, const uint8_t col) {
	uint8_t tmp = colour;
	colour = col;
	putc(c);
	colour = tmp;
}

void puts_size(const char* data, const size_t size) {
	for (size_t i = 0; i < size; i++)
		putc(data[i]);
}

void puts_coloured(const char* data, const uint8_t col) {
	uint8_t tmp = colour;
	colour = col;
	puts_size(data, strlen(data));
	colour = tmp;
}

void puts(const char* data) {
	puts_size(data, strlen(data));
}

void puts_word(const uint16_t* data) {
	int i = 0;
	while (data[i++] != 0x0000)
		putc(data[i]);
}

void print_int(int num, uint8_t colour) {
	if (num == 0) {
		putc_coloured('0',colour);
		return;
	}

	if (num < 0) {
		putc_coloured('-',colour);
		num = -num;
	}

	char buffer[10];  // Buffer to hold the digits
	int i = 0;

	while (num > 0) {
		buffer[i++] = (num % 10) + '0';  // Convert digit to character
		num /= 10;
	}

	// Print the digits in reverse order
	while (--i >= 0) {
		putc_coloured(buffer[i],colour);
	}
}

void scroll_terminal() {
    for (size_t y = 1; y < VGA_HEIGHT; ++y) {
        for (size_t x = 0; x < VGA_WIDTH; ++x) {
            txtbuff[(y - 1) * VGA_WIDTH + x] = txtbuff[y * VGA_WIDTH + x];
        }
    }

    for (size_t x = 0; x < VGA_WIDTH; ++x) {
        txtbuff[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = vga_entry(' ', colour);
    }

    txty = VGA_HEIGHT - 1;
    txtx = 0;
    update_cursor(txtx, txty);
}
