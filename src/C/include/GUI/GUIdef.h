#pragma once
#include "typedef.h"
byte_t* current_buffer=(byte_t*)0xA0000;

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

