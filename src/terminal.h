void putc(const uint16_t c);

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

uint8_t colour;
uint16_t* txtbuff;

size_t txty=0;
size_t txtx=0;
size_t tabw = 4;
bool numlock=false;

const uint16_t keyboard_map[256] = {
	0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', 0xF003,
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0xF001, 'a', 's',
	'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '#', 'z', 'x', 'c', 'v',
	'b', 'n', 'm', ',', '.', '/', 0xF002, '*', 0xF002, ' ',
	0xF001, 0xF010, 0xF011, 0xF012, 0xF013, 0xF014, 0xF015, 0xF016, 0xF017, 0xF018, 0xF019, 0xF004, '/', '7',
	'8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\n',
	0xF002, '\\', 0xF0, 0xF0
};

const uint16_t keyboard_map_shifted[256] = {
	0, 27, '!', '"', '\\', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t',
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 'C', 'A', 'S',
	'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '@', '`', 0, '#', 'Z', 'X', 'C', 'V',
	'B', 'N', 'M', '<', '>', '?', 0xF002, '*', 0xF002, ' ',
	0xF001, 0xF010, 0xF011, 0xF012, 0xF013, 0xF014, 0xF015, 0xF016, 0xF017, 0xF018, 0xF019, 0xF004, '/', '7',
	'8', '9', '-', '4', '5', '6', '+', '1', '2', '3', '0', '.', '\n',
	0xF002, '\\', 0xF0, 0xF0
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
	}

	return (scancode & 0x80) ? 0 : (shift_pressed ? keyboard_map_shifted[scancode] : keyboard_map[scancode]);
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

void putch(const uint16_t c, const uint8_t colour, const size_t txtx, const size_t txty) {
	if (c == '\b') return;
	if (c == 0) return;
	if (c == '\n') return;
	if (c == 0xF003) {for (size_t i = 0; i < tabw; ++i) putch(' ', colour, txtx, txty); return;}

	((char*)videobuff)[(txty*80*2)+(txtx*2)] = c;
	((char*)videobuff)[(txty*80*2)+(txtx*2)+1] = colour;
	outb(0xe9, c);
}

void putc(const uint16_t c) {
	if (txty < 0) txty = 0;
	if (txtx < 0) txtx = 0;

	if (c == '\b') {--txtx;
		if (txtx < 0) {
			txtx = 0; --txty;
			while ( ((char*)videobuff)[(txty*80*2) + (txtx*2)] != '\0') {txtx++;}
		}
		((char*)videobuff)[(txty*80*2)+(txtx*2)] = '\0';
		((char*)videobuff)[(txty*80*2)+(txtx*2)+1] = colour;
		outb(0xe9, c);
		update_cursor(txtx, txty);
		return;
	}

	if (c == 0) return;
	if (c == '\n') {txtx = 0; txty++; update_cursor(txtx, txty); return;}
	if (c == 0xF003) {for (size_t i = 0; i < tabw; ++i) putc(' '); return;}

	((char*)videobuff)[(txty*80*2)+(txtx*2)] = c;
	((char*)videobuff)[(txty*80*2)+(txtx*2)+1] = colour;
	if (txtx <= 80) {
		txtx++;
	} else {
		txtx = 0; txty++;
	}
	outb(0xe9, c);
	update_cursor(txtx, txty);
}

void putcc(const char c, const uint8_t col) {
	uint8_t tmp = colour;
	colour = col;
	putc(c);
	colour = tmp;
}

void putss(const char* data, const size_t size) {
	for (size_t i = 0; i < size; i++)
		putc(data[i]);
}

void putsc(const char* data, const uint8_t col) {
	uint8_t tmp = colour;
	colour = col;
	putss(data, strlen(data));
	colour = tmp;
}

void puts(const char* data) {
	putss(data, strlen(data));
}

void putsw(const uint16_t* data) {
	int i = 0;
	while (data[i++] != 0x0000)
		putc(data[i]);
}