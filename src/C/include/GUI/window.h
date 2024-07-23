#pragma once
#include "GUI/GUIdef.h"

static bool_t redraw=true;
static uint32_t mousex=50,mousey=50;
static uint8_t mouses = 2;

static bool_t resize=false;
static bool_t moving=false;

static bool_t windowmngr=false;

window_t* active_window = NULL;
windows_t* GUIWindows = &windows;

int add_window(window_t* window) {
	// find the first empty element
	int i=0;
	for (i = 0; GUIWindows->windows[i] != NULL && i < MAX_WINDOW_COUNT+1; ++i);
	if(i==MAX_WINDOW_COUNT)return -1;

	window->idx = i;
	GUIWindows->windows[i] = window;
	return 0;
}

void remove_window(window_t* win, windows_t* windows) {
	for (size_t i = 0; i < MAX_ELEMENT_COUNT; ++i) { // remove elems
		switch(win->elements[i]->type) {
			case elemtext:
				remove_text_element(win, win->elements[i]);
				break;
			case elembitmap:
				remove_bitmap_element(win, win->elements[i]);
				break;
			case elembutton:
				remove_button_element(win, win->elements[i]);
				break;
			default:
				break;
		}
	}

	size_t x;
	for (x = 0; x < MAX_WINDOW_COUNT; ++x) {
		if (windows->windows[x] == win) {windows->windows[x] = NULL; break;}
	}

	free(win);
}

void drawWindow(window_t* window) {
	drawRectWOutline((rect_t*)window, 250, 245); // main

	rect_t controlRect = {
		.x = window->x,
		.y = window->y,
		.w = window->w,
		.h = CONTROL_HEIGHT,
	};
	drawRectWOutline(&controlRect, 0b00000111, 245);

	rect_t contentRect = {
		.x = window->x + WIN_MARGIN,
		.y = window->y + CONTROL_HEIGHT,
		.w = window->w - WIN_MARGIN*2,
		.h = window->h - CONTROL_HEIGHT - WIN_MARGIN,
	};
	fillRect(&contentRect, 255);

	rect_t closeButtonRect = {
		.x = window->x + window->w - CLOSE_BUTTON_SIZE - WIN_MARGIN,
		.y = window->y + WIN_MARGIN,
		.w = CLOSE_BUTTON_SIZE,
		.h = CLOSE_BUTTON_SIZE,
	};

	fillRect(&closeButtonRect, 0b11001000);

	// Optionally, draw an 'X' or any other symbol on the close button
	// For simplicity, let's draw a cross symbol ('X')
	drawLine(closeButtonRect.x + CLOSE_BUTTON_XMARGIN, closeButtonRect.y + CLOSE_BUTTON_XMARGIN,
			 closeButtonRect.x + closeButtonRect.w - CLOSE_BUTTON_XMARGIN, closeButtonRect.y + closeButtonRect.h - CLOSE_BUTTON_XMARGIN, 
			 0);  // Example: draw a black 'X'
	drawLine(closeButtonRect.x + closeButtonRect.w - CLOSE_BUTTON_XMARGIN, closeButtonRect.y + CLOSE_BUTTON_XMARGIN, 
			 closeButtonRect.x + CLOSE_BUTTON_XMARGIN, closeButtonRect.y + closeButtonRect.h - CLOSE_BUTTON_XMARGIN, 
			 0);  // Example: draw a black 'X'
	draw_textw(membuff, window, window->name, window->x + WIN_MARGIN,window->y + WIN_MARGIN, 254, FONT_WIDTH + CLOSE_BUTTON_SIZE * 2, false);

	// elements
	for (int i = 0; i < MAX_ELEMENT_COUNT; ++i) {
		element_t* elem = window->elements[i];
		if(elem->exists) {
			switch (elem->type) {
				case elemtext: {
					draw_text_element(membuff, window, elem);
					break;
				}

				case elembitmap: {
					draw_bitmap_element(membuff, window, elem);
				}

				case elembutton: {
					draw_button_element(membuff, window, elem);
				}

				default: {break;}
			}
		}
	}
}

window_t* find_window(windows_t* GUIWindows, uint32_t x, uint32_t y) {
	for (size_t i = 0; i < MAX_WINDOW_COUNT; ++i) {
		if (x >= GUIWindows->windows[i]->x && x < GUIWindows->windows[i]->x + GUIWindows->windows[i]->w &&
			y >= GUIWindows->windows[i]->y && y < GUIWindows->windows[i]->y + GUIWindows->windows[i]->h) {

			return GUIWindows->windows[i];
		}
	}

	return NULL;
}

window_t* handle_drag(windows_t* GUIWindows, uint32_t x, uint32_t y) {
	for (size_t i = 0; i < MAX_WINDOW_COUNT; ++i) {
		if (x >= GUIWindows->windows[i]->x && x < GUIWindows->windows[i]->x + GUIWindows->windows[i]->w - CLOSE_BUTTON_SIZE - 5 &&
			y >= GUIWindows->windows[i]->y && y < GUIWindows->windows[i]->y + CONTROL_HEIGHT) {

			active_window = (active_window == NULL) ? GUIWindows->windows[i] : NULL;
			moving = !moving;
			return GUIWindows->windows[i];
		}
	}

	return NULL;
}

window_t* handle_resize(windows_t* GUIWindows, uint32_t x, uint32_t y) {
	for (size_t i = 0; i < MAX_WINDOW_COUNT; ++i) {
		if (x >= GUIWindows->windows[i]->x + GUIWindows->windows[i]->w - 5 && x < GUIWindows->windows[i]->x + GUIWindows->windows[i]->w &&
			y >= GUIWindows->windows[i]->y + GUIWindows->windows[i]->h - 5 && y < GUIWindows->windows[i]->y + GUIWindows->windows[i]->h) {

			active_window = active_window == NULL ? GUIWindows->windows[i] : NULL;
			resize = !resize;
			return GUIWindows->windows[i];
		}
	}

	return NULL;
}

window_t* handle_close(windows_t* GUIWindows, uint32_t x, uint32_t y) {
	for (size_t i = 0; i < MAX_WINDOW_COUNT; ++i) {
		if (x >= GUIWindows->windows[i]->x + GUIWindows->windows[i]->w - CLOSE_BUTTON_SIZE - WIN_MARGIN && x < GUIWindows->windows[i]->x + GUIWindows->windows[i]->w - WIN_MARGIN &&
			y >= GUIWindows->windows[i]->y + WIN_MARGIN && y < GUIWindows->windows[i]->y + CLOSE_BUTTON_SIZE + WIN_MARGIN) {

			free((byte_t*)GUIWindows->windows[i]);
			GUIWindows->windows[i] = (window_t*)NULL;
			redraw = true;
			return GUIWindows->windows[i];
		}
	}

	return NULL;
}

element_t* handle_elems(windows_t* GUIWindows, uint32_t x, uint32_t y) {
	window_t* win = find_window(GUIWindows, x, y);

	if (!win) return NULL;

	for (size_t i = 0; i < MAX_ELEMENT_COUNT; ++i) {
		element_t* elem = win->elements[i];
		if (!elem) continue;

		switch(elem->type) {
			case elembutton:
				if (x >= win->x + elem->x && x < win->x + elem->x + ((button_t*)elem->data.raw)->w &&
					y >= win->y + elem->y && y < win->y + elem->y + ((button_t*)elem->data.raw)->h + CONTROL_HEIGHT) {
					
					((button_t*)elem->data.raw)->onClick((void*)win, (void*)elem);
					redraw = true;
					return elem;
				}
				break;
			case elemtextinp:
				if (x >= win->x + elem->x && x < win->x + elem->x + ((button_t*)elem->data.raw)->w &&
					y >= win->y + elem->y && y < win->y + elem->y + ((button_t*)elem->data.raw)->h + CONTROL_HEIGHT) {
					
					
					redraw = true;
					return elem;
				}
				break;
			default:
				continue;
		}
	}

	return NULL;
}

void handle_updates(windows_t* GUIWindows) {
	for (size_t i = 0; i < MAX_WINDOW_COUNT; ++i) {
		if (GUIWindows->windows[i] != NULL) {
			if (GUIWindows->windows[i]->main_ran == false) {
				GUIWindows->windows[i]->main_ran = false;
				GUIWindows->windows[i]->Main(GUIWindows->windows[i]);
			}
			GUIWindows->windows[i]->Update(GUIWindows->windows[i]);
		}
	}
}

window_t* init_window(int w, int h, wchar_t name[], int (*Main)(void*), void (*Update)(void*)) {
	if (Main == NULL || Update == NULL) return NULL;
	window_t* win = malloc(sizeof(window_t));
	win->w = w;
	win->h = h;
	win->x = 10;
	win->y = 10;
	win->Main = Main;
	win->Update = Update;
	win->main_ran = false;

	wstrcpy(win->name, name);

	return win;
}

void draw_bg(byte_t* membuff, byte_t* data) {
	if(!data) return;
	for (size_t y = 0, i = 0; y < 200; ++y) {
		for (size_t x = 0; x < 320; ++x, ++i) {
			putp(membuff, x, y, data[i]);
		}
	}
}
