#pragma once
#include "typedef.h"

static bool_t redraw=true;
static uint32_t mousex=50,mousey=50;
static uint8_t mouses = 2;

static bool_t resize=false;
static bool_t moving=false;

window_t* active_window = NULL;

int add_window(window_t* window) {
	// find the first empty element
	int i=0;
	for (i = 0; windows.windows[i] != NULL && i < MAX_WINDOW_COUNT+1; ++i);
	if(i==MAX_WINDOW_COUNT)return -1;

	window->idx = i;
	windows.windows[i] = window;
	return 0;
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

				default: {break;}
			}
		}
	}
}

window_t* handle_drag(windows_t* windows, uint32_t x, uint32_t y) {
	for (size_t i = 0; i < MAX_WINDOW_COUNT; ++i) {
		if (x >= windows->windows[i]->x && x < windows->windows[i]->x + windows->windows[i]->w - CLOSE_BUTTON_SIZE - 5 &&
			y >= windows->windows[i]->y && y < windows->windows[i]->y + CONTROL_HEIGHT) {

			active_window = (active_window == NULL) ? windows->windows[i] : NULL;
			moving = !moving;
			return windows->windows[i];
		}
	}

	return NULL;
}

window_t* handle_resize(windows_t* windows, uint32_t x, uint32_t y) {
	for (size_t i = 0; i < MAX_WINDOW_COUNT; ++i) {
		if (x >= windows->windows[i]->x + windows->windows[i]->w - 5 && x < windows->windows[i]->x + windows->windows[i]->w &&
			y >= windows->windows[i]->y + windows->windows[i]->h - 5 && y < windows->windows[i]->y + windows->windows[i]->h) {

			active_window = active_window == NULL ? windows->windows[i] : NULL;
			resize = !resize;
			return windows->windows[i];
		}
	}

	return NULL;
}

window_t* handle_close(windows_t* windows, uint32_t x, uint32_t y) {
	for (size_t i = 0; i < MAX_WINDOW_COUNT; ++i) {
		if (x >= windows->windows[i]->x + windows->windows[i]->w - CLOSE_BUTTON_SIZE - WIN_MARGIN && x < windows->windows[i]->x + windows->windows[i]->w - WIN_MARGIN &&
			y >= windows->windows[i]->y + WIN_MARGIN && y < windows->windows[i]->y + CLOSE_BUTTON_SIZE + WIN_MARGIN) {

			free((byte_t*)windows->windows[i]);
			windows->windows[i] = (window_t*)NULL;
			redraw = true;
			return windows->windows[i];
		}
	}

	return NULL;
}

window_t* init_window(int w, int h, wchar_t name[]) {
	window_t* win = malloc(sizeof(window_t));
	win->w = w;
	win->h = h;
	win->x = 10;
	win->y = 10;
	
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

void window_mngr(windows_t* windows) {
	while (true) {
		if (redraw) {
			memset(membuff, DESKTOP_C, 320*200);
			for (size_t i = 0; i < MAX_WINDOW_COUNT; ++i) {
				if (windows->windows[i])
					drawWindow(windows->windows[i]);
				if (active_window) drawWindow(active_window);
			}

			for (int y=0; y < CURS_H; ++y) {
				for (int x=0; x < CURS_W; ++x) {
					if (curs[y][x] == 1)
						putp(membuff, mousex+x, mousey+y, 240);
					else if (curs[y][x] == 2)
						putp(membuff, mousex+x, mousey+y, 255);
				}
			}

			update_screen(membuff);
			redraw = false;
		}

		switch (getch()) {
			case KEY_LE:
				mousex-=mouses;
				if (active_window != NULL && moving) active_window->x -= mouses;
				if (active_window != NULL && resize) {active_window->w -= mouses; if (active_window->w < MIN_WIN_W) {active_window->w = MIN_WIN_W; moving = false; resize = false; active_window = NULL; } }
				redraw = true; break;
			case KEY_RI:
				mousex+=mouses;
				if (active_window != NULL && moving) active_window->x += mouses;
				if (active_window != NULL && resize) {active_window->w += mouses; if (active_window->w < MIN_WIN_W) {active_window->w = MIN_WIN_W; moving = false; resize = false; active_window = NULL; } }
				redraw = true; break;
			case KEY_UP:
				mousey-=mouses;
				if (active_window != NULL && moving) active_window->y -= mouses;
				if (active_window != NULL && resize) {active_window->h -= mouses; if (active_window->h < MIN_WIN_H) {active_window->h = MIN_WIN_H; moving = false; resize = false; active_window = NULL; } }
				redraw = true; break;
			case KEY_DN:
				mousey+=mouses;
				if (active_window != NULL && moving) active_window->y += mouses;
				if (active_window != NULL && resize) {active_window->h += mouses; if (active_window->h < MIN_WIN_H) {active_window->h = MIN_WIN_H; moving = false; resize = false; active_window = NULL; } }
				redraw = true; break;
			case KEY_INTERACT: {
				handle_drag(windows, mousex, mousey);
				handle_resize(windows, mousex, mousey);
				handle_close(windows, mousex, mousey);
				break;
			}
		}
	}
}

void init_graphics() {
	current_buffer = membuff;

	/* Initialize terminal interface */
	VGAMode(3, GVGA_WIDTH, GVGA_HEIGHT,1); // Set VGA mode to 320x200 with 256 colors

	// Define the palette array for 256 colors
	char PAL256[256 * 3];
	size_t x = 0;

	// Create a color palette for the first 240 colors
	for (int r = 0; r < 8; ++r) {
		for (int g = 0; g < 8; ++g) {
			for (int b = 0; b < 4; ++b) {
				if (x < 240 * 3) {
					PAL256[x++] = (r << 2); // Red component
					PAL256[x++] = (g << 2); // Green component
					PAL256[x++] = (b << 3); // Blue component
				}
			}
		}
	}

	// Create a grayscale palette for the last 16 colors
	for (int i = 0; i < 16; ++i) {
		unsigned char gray = (i * 4);
		PAL256[x++] = gray; // Red component
		PAL256[x++] = gray; // Green component
		PAL256[x++] = gray; // Blue component
	}

	VGASetPal(PAL256, 0, 256);
}