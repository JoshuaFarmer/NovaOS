#pragma once
#include "GUI/GUIdef.h"
#include "interrupts.h"
#include "GUI/text.h"
#include "GUI/element.h"
#include "GUI/graphics.h"
#include "GUI/video.h"
#include "GUI/window.h"
#include "GUI/taskbar.h"

int example_main(void* ptr) {
	if (ptr == NULL) return -1;

	window_t* window = (window_t*)ptr;
	create_text_element(window, L"TEST #1", 10, 10);
	
	return 0;
}

void example_update(void* ptr) {
	draw_text(membuff, ((window_t*)ptr)->name, 0, 0, 0b11110000);
	return;
}

void window_mngr0() {
	if (windowmngr) {
			memset(membuff, DESKTOP_C, 320*200);
			for (size_t i = 0; i < MAX_WINDOW_COUNT; ++i) {
				if (GUIWindows->windows[i])
					drawWindow(GUIWindows->windows[i]);
				if (active_window) drawWindow(active_window);
			}
			drawTaskbar();

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

		handle_updates(GUIWindows);
	}
}

void window_mngr(windows_t* wins) {
	GUIWindows = wins;
	windowmngr = true;
	sti();
	default_exception();
	while (true) {
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
			case KEY_INS: {
				handle_drag(GUIWindows, mousex, mousey);
				handle_resize(GUIWindows, mousex, mousey);
				handle_close(GUIWindows, mousex, mousey);
				handle_elems(GUIWindows, mousex, mousey);
				break;
			}
		}

		if (mousex > 320) mousex = 0;
		if (mousey > 200) mousey = 0;
	}
}

void start_gui(void) {
	init_gdt();
	init_idt();
	init_heap();
	init_graphics();

	//window_t *window1 = init_window(96, 144, L"test window 1", example_main, example_update);

	//if(add_window(window1)) draw_text(membuff, L"BRUH", 0, 0, 0);
	window_mngr(&windows);
}