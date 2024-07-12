#pragma once

#define GVGA_WIDTH 320
#define GVGA_HEIGHT 200
#define CLOSE_BUTTON_SIZE 8
#define CLOSE_BUTTON_XMARGIN 3
#define CONTROL_HEIGHT 15
#define WIN_MARGIN 5

#define MAX_WINDOW_COUNT 25
#define MAX_ELEMENT_COUNT 25

#define MIN_WIN_W 32
#define MIN_WIN_H 24

#define DESKTOP_C 230

#define NULL (void*)0

#define true 1
#define false 0

#define update_screen(src) memcpy((void*)0xA0000, src, GVGA_WIDTH*GVGA_HEIGHT);
#define putp(buff, x, y, c) buff[(GVGA_WIDTH*(y)) + (x)] = c

#define CURS_W 10
#define CURS_H 12
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT 0x60

#define vgabuff 0xA0000
#define videobuff 0xB8000
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_DATA_PORT 0x60
#define STATUS_BSY	  0x80
#define STATUS_RDY	  0x40
#define STATUS_DRQ	  0x08
#define STATUS_DF		0x20
#define STATUS_ERR	  0x01
#define read_sector(drive, sector, buffer) LBA28_read_sector(drive, sector, 1, buffer)
#define write_sector(drive, sector, data) LBA28_write_sector(drive, sector, 1, data)
#define PIT_CHANNEL0_PORT 0x40
#define PIT_COMMAND_PORT 0x43
#define SECTOR_SIZE 512
#define ROOT_DIR_SECTOR 2
#define RESERVED_SECTORS 32
#define FAT_SECTORS 32
#define cli() asm("cli");
#define sti() asm("sti");

typedef unsigned long long  uint64_t;
typedef unsigned int   uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char  uint8_t;

typedef long long int64_t;
typedef int   int32_t;
typedef short int16_t;
typedef char  int8_t;

typedef uint8_t  byte_t;
typedef uint16_t word_t;
typedef uint32_t dword_t;
typedef uint64_t qword_t;

typedef int8_t  sbyte_t;
typedef int16_t sword_t;
typedef int32_t sdword_t;
typedef int64_t sqword_t;

typedef int32_t wchar_t;

typedef uint64_t size_t;
typedef uint8_t bool_t;

typedef uint32_t error_t;
typedef uint8_t BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef uint64_t QWORD;
typedef WORD WCHAR;
typedef uint32_t UINT;
typedef uint32_t LBA_t;
typedef uint32_t FSIZE_t;

byte_t* current_buffer=(byte_t*)0xA0000;

#define vga_color vga_colour

enum vga_colour {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

enum {
	KEY_TAB = '\t',
	KEY_NULL = 0xfff, KEY_ESC, KEY_CTRL, KEY_ALT,
	KEY_NUML, KEY_SCRL, KEY_HOM,
	KEY_UP, KEY_PGU, KEY_LE, KEY_RI, KEY_END, KEY_DN, KEY_PGD, KEY_INS, KEY_DEL,
	
	KEY_F1,
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

	KEY_INTERACT,
} keys;

typedef enum uint32_t {
	elemtext,
	elembitmap,
	elembutton,
	elemtextinp,
} element_type_t;

typedef struct {
	byte_t*  data;
	uint16_t w,h;
} image_t;

typedef struct {
	void (*onClick)(void*,void*);
	uint16_t w,h;
} button_t;

typedef struct {
	byte_t* raw;
	size_t  length;
} data_t;

typedef struct {
	data_t   data; // e.g. ptr to function for button, or ptr to image
	wchar_t* text;
	uint16_t x;
	uint16_t y;

	element_type_t type;
	size_t idx;
	bool_t exists;
} element_t;

typedef struct {
	uint32_t x,y,w,h;
} rect_t;

typedef struct {
	uint32_t x,y,w,h;
	wchar_t name[32];
	element_t* elements[MAX_ELEMENT_COUNT];

	size_t idx;

	int  (*Main)(void*); // passed value is window ptr
	bool_t main_ran;
	
	void (*Update)(void*);
} window_t;

typedef struct {
	window_t *windows[MAX_WINDOW_COUNT];
} windows_t;

windows_t windows;
byte_t membuff[GVGA_WIDTH*GVGA_HEIGHT];

void draw_text(byte_t* membuff, wchar_t* s, int x, int y, int colour);
void draw_textw(byte_t* membuff, window_t* win, wchar_t* s, uint32_t x, uint32_t y, int colour, int xmar, bool_t wrap);
void draw_text_element(byte_t* membuff, window_t* win, element_t* elem);
element_t* create_text_element(window_t* win, wchar_t* text, uint16_t x, uint16_t y);
void remove_text_element(window_t* win, element_t* elem);
void draw_bitmap_element(byte_t* membuff, window_t* win, element_t* elem);
void remove_bitmap_element(window_t* win, element_t* elem);
element_t* create_bitmap_element(window_t* win, byte_t* data, uint16_t w, uint16_t h, uint16_t x, uint16_t y);

const byte_t curs[CURS_H][CURS_W] = {
	{0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
	{0, 1, 2, 1, 0, 0, 0, 0, 0, 0},
	{0, 1, 2, 2, 1, 0, 0, 0, 0, 0},
	{0, 1, 2, 2, 2, 1, 0, 0, 0, 0},
	{0, 1, 2, 2, 2, 2, 1, 0, 0, 0},
	{0, 1, 2, 2, 2, 2, 1, 0, 0, 0},
	{0, 1, 2, 2, 2, 1, 0, 0, 0, 0},
	{0, 1, 2, 1, 2, 1, 0, 0, 0, 0},
	{0, 1, 1, 0, 1, 2, 1, 0, 0, 0},
	{0, 0, 0, 0, 1, 2, 1, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

